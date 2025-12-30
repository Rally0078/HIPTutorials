cmake_minimum_required(VERSION 4.1)
set(CMAKE_MAKE_PROGRAM_PATH_NO_SHORT_NAMES ON)
# For 9070 XT
set(CMAKE_HIP_ARCHITECTURES "gfx1201")
if(WIN32)
    set(ROCM_PATH "C:/Program Files/AMD/ROCm/6.4" CACHE PATH "Path to ROCm root")
    list(APPEND CMAKE_PREFIX_PATH "${ROCM_PATH}")
endif()
project(hiptutorials LANGUAGES CXX HIP)

find_package(hip REQUIRED)
set(CMAKE_CXX_STANDARD 20)

# Add GPU kernels
file(GLOB_RECURSE KERNEL_SOURCES "kernels/*.hip")
add_library(gpu_kernels STATIC ${KERNEL_SOURCES})
target_include_directories(gpu_kernels PUBLIC include)
target_link_libraries(gpu_kernels PRIVATE hip::host)

# Base interface for each experiment file
add_library(experiment_base INTERFACE)
target_link_libraries(experiment_base INTERFACE gpu_kernels hip::host)
target_include_directories(experiment_base INTERFACE include)

file(GLOB EXPERIMENT_SOURCES "experiments/*.cpp")
foreach(EXE_SOURCE ${EXPERIMENT_SOURCES})
    # Get the filename without extension
    get_filename_component(EXE_NAME ${EXE_SOURCE} NAME_WLE)

    # Create the executable
    add_executable(${EXE_NAME} ${EXE_SOURCE})

    # Link the executable to our GPU kernels and HIP runtime through interface
    target_link_libraries(${EXE_NAME} PRIVATE experiment_base)
endforeach()
