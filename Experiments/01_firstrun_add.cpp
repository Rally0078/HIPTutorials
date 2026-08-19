#include <utils/kernels.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
#include <random>
#include <algorithm>
#include <utils/ops.hpp>

void randomize_vectors(std::vector<float>& vec, float min, float max, unsigned int seed) {
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(min, max);

    // Fill the vector with random numbers
    std::ranges::generate(vec, [&]() { return dist(gen); });
}

int main() {
    constexpr long n = 500'000'000;
    constexpr size_t bytes = n * sizeof(float);
    int device;
    cuda_tutorials::check_cuda(cudaGetDevice(&device));

    cudaDeviceProp props;
    cuda_tutorials::check_cuda(cudaGetDeviceProperties(&props, device));

    std::cout << "--- GPU DEVICE INFO ---" << std::endl;
    std::cout << "Device Name: " << props.name << std::endl;
#if defined(GPU_BACKEND_HIP) || defined(__HIP_PLATFORM_AMD__) || defined(HIP)
    std::cout << "Architecture (gcnArchName): " << props.gcnArchName << std::endl;
#else
    std::cout << "Compute Capability: " << props.major << "." << props.minor << std::endl;
#endif
    std::cout << "-----------------------" << std::endl;

    std::vector<float> a(n), b(n), c(n);
    randomize_vectors(a, 0.0f, 1.0f, 12345);
    randomize_vectors(b, 0.0f, 1.0f, 54321);

    float *d_a, *d_b, *d_c;
    cuda_tutorials::check_cuda(cudaMalloc(&d_a, bytes));
    cuda_tutorials::check_cuda(cudaMalloc(&d_b, bytes));
    cuda_tutorials::check_cuda(cudaMalloc(&d_c, bytes));

    cuda_tutorials::check_cuda(cudaMemcpy(d_a, a.data(), bytes, cudaMemcpyHostToDevice));
    cuda_tutorials::check_cuda(cudaMemcpy(d_b, b.data(), bytes, cudaMemcpyHostToDevice));
    cuda_tutorials::check_cuda(cudaMemcpy(d_c, c.data(), bytes, cudaMemcpyHostToDevice));

    // Call the wrapper function
    cuda_tutorials::launch_vector_add(d_a, d_b, d_c, n);
    if (const auto err = cudaGetLastError(); err != cudaSuccess) {
        std::cerr << "Launch Error: " << cudaGetErrorString(err) << std::endl;
    }

    cuda_tutorials::check_cuda(cudaDeviceSynchronize());
    cuda_tutorials::check_cuda(cudaMemcpy(c.data(), d_c, bytes, cudaMemcpyDeviceToHost));

    std::cout << "Kernel launched from .cpp file!" << std::endl;
    std::cout << "Value c[10] = " << c[10] << " = " << a[10] << " + " << b[10] << std::endl;
    std::cin.get();

    cuda_tutorials::check_cuda(cudaFree(d_a));
    cuda_tutorials::check_cuda(cudaFree(d_b));
    cuda_tutorials::check_cuda(cudaFree(d_c));

    return 0;
}