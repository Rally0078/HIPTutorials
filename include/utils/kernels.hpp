#ifndef KERNELS_H
#define KERNELS_H

#include <iostream>
#include <source_location>

#if defined(GPU_BACKEND_HIP) || defined(__HIP_PLATFORM_AMD__) || defined(HIP)
#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>

using cudaError_t = hipError_t;
constexpr auto cudaSuccess = hipSuccess;
using cudaDeviceProp = hipDeviceProp_t;

#define cudaMalloc hipMalloc
#define cudaFree hipFree
#define cudaMemcpy hipMemcpy
#define cudaMemcpyHostToDevice hipMemcpyHostToDevice
#define cudaMemcpyDeviceToHost hipMemcpyDeviceToHost
#define cudaGetDevice hipGetDevice
#define cudaGetDeviceProperties hipGetDeviceProperties
#define cudaGetLastError hipGetLastError
#define cudaGetErrorString hipGetErrorString
#define cudaDeviceSynchronize hipDeviceSynchronize
#else
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#endif

namespace cuda_tutorials {

    inline void check_cuda(cudaError_t status,
                          const std::source_location loc = std::source_location::current()) {
        if (status != cudaSuccess) {
            std::cerr << "CUDA Error: " << cudaGetErrorString(status) << "\n"
                      << "Location: " << loc.file_name() << ":" << loc.line() << "\n"
                      << "Function: " << loc.function_name() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

#endif