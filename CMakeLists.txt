cmake_minimum_required(VERSION 3.21)

set(CMAKE_MAKE_PROGRAM_PATH_NO_SHORT_NAMES ON)

# Switchable option for GPU backend: "CUDA" (default) or "HIP"
set(GPU_BACKEND "CUDA" CACHE STRING "GPU Backend to use: CUDA or HIP")
set_property(CACHE GPU_BACKEND PROPERTY STRINGS "CUDA" "HIP")

project(gpututorials LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE KERNEL_SOURCES "kernels/*.cu")

if(GPU_BACKEND STREQUAL "CUDA")
    if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
        set(CMAKE_CUDA_ARCHITECTURES "86")
    endif()

    enable_language(CUDA)

    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)

    find_package(CUDAToolkit REQUIRED)

    set_source_files_properties(${KERNEL_SOURCES} PROPERTIES LANGUAGE CUDA)

    add_library(gpu_kernels STATIC ${KERNEL_SOURCES})
    target_include_directories(gpu_kernels PUBLIC include ${CUDAToolkit_INCLUDE_DIRS})
    target_link_libraries(gpu_kernels PUBLIC CUDA::cudart)
    target_compile_definitions(gpu_kernels PUBLIC GPU_BACKEND_CUDA)

    add_library(experiment_base INTERFACE)
    target_link_libraries(experiment_base INTERFACE gpu_kernels CUDA::cudart)
    target_include_directories(experiment_base INTERFACE include ${CUDAToolkit_INCLUDE_DIRS})
    target_compile_definitions(experiment_base INTERFACE GPU_BACKEND_CUDA)

elseif(GPU_BACKEND STREQUAL "HIP")
    if(NOT DEFINED CMAKE_HIP_ARCHITECTURES)
        set(CMAKE_HIP_ARCHITECTURES "gfx1201")
    endif()
    if(WIN32)
        set(ROCM_PATH "C:/Program Files/AMD/ROCm/6.4" CACHE PATH "Path to ROCm root")
        list(APPEND CMAKE_PREFIX_PATH "${ROCM_PATH}")
    endif()

    enable_language(HIP)
    find_package(hip REQUIRED)

    set_source_files_properties(${KERNEL_SOURCES} PROPERTIES LANGUAGE HIP)

    add_library(gpu_kernels STATIC ${KERNEL_SOURCES})
    target_include_directories(gpu_kernels PUBLIC include ${ROCM_PATH}/include ${hip_INCLUDE_DIRS})
    target_link_libraries(gpu_kernels PUBLIC hip::host)
    target_compile_definitions(gpu_kernels PUBLIC GPU_BACKEND_HIP __HIP_PLATFORM_AMD__)

    add_library(experiment_base INTERFACE)
    target_link_libraries(experiment_base INTERFACE gpu_kernels hip::host)
    target_include_directories(experiment_base INTERFACE include ${ROCM_PATH}/include ${hip_INCLUDE_DIRS})
    target_compile_definitions(experiment_base INTERFACE GPU_BACKEND_HIP __HIP_PLATFORM_AMD__)

else()
    message(FATAL_ERROR "Invalid GPU_BACKEND '${GPU_BACKEND}'. Must be 'CUDA' or 'HIP'.")
endif()

file(GLOB EXPERIMENT_SOURCES "Experiments/*.cpp")

foreach(EXE_SOURCE ${EXPERIMENT_SOURCES})
    get_filename_component(EXE_NAME ${EXE_SOURCE} NAME_WLE)
    add_executable(${EXE_NAME} ${EXE_SOURCE})
    target_link_libraries(${EXE_NAME} PRIVATE experiment_base)
endforeach()
