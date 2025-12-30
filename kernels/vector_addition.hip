#include <hip/hip_runtime.h>
#include <utils/kernels.hpp>

__global__ void vector_add_kernel(const float* a, const float* b, float* c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) c[i] = a[i] + b[i];
}

namespace hip_tutorials {
    extern "C" void launch_vector_add(const float* a, const float* b, float* c, int n) {
        int threadsPerBlock = 256;
        int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
        vector_add_kernel<<<blocksPerGrid, threadsPerBlock>>>(a, b, c, n);
    }
}
