#include <hip/hip_runtime_api.h> // Use _api.h for standard C++ files
#include <iostream>
#include <vector>
#include <functional>
#include <numeric>
#include <random>
#include <algorithm>
#include <utils/kernels.hpp>
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
    hip_tutorials::check_hip(hipGetDevice(&device));

    hipDeviceProp_t props;
    hip_tutorials::check_hip(hipGetDeviceProperties(&props, device));

    std::cout << "--- GPU DEVICE INFO ---" << std::endl;
    std::cout << "Device Name: " << props.name << std::endl;
    std::cout << "Architecture (gcnArchName): " << props.gcnArchName << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::vector<float> a(n), b(n), c(n);
    randomize_vectors(a,0.0f,1.0f,12345);
    randomize_vectors(b,0.0f,1.0f,54321);
    float *d_a, *d_b, *d_c;
    hip_tutorials::check_hip(hipMalloc(&d_a, bytes));
    hip_tutorials::check_hip(hipMalloc(&d_b, bytes));
    hip_tutorials::check_hip(hipMalloc(&d_c, bytes));

    hip_tutorials::check_hip(hipMemcpy(d_a, a.data(), bytes, hipMemcpyHostToDevice));
    hip_tutorials::check_hip(hipMemcpy(d_b, b.data(), bytes, hipMemcpyHostToDevice));
    hip_tutorials::check_hip(hipMemcpy(d_c, c.data(), bytes, hipMemcpyHostToDevice));

    // Call the wrapper function instead of the kernel directly
    hip_tutorials::launch_vector_add(d_a, d_b, d_c, n);
    if (const auto err = hipGetLastError(); err != hipSuccess) {
        std::cerr << "Launch Error: " << hipGetErrorString(err) << std::endl;
    }
    hip_tutorials::check_hip(hipDeviceSynchronize());
    hip_tutorials::check_hip(hipMemcpy(c.data(), d_c, bytes, hipMemcpyDeviceToHost));
    hip_tutorials::check_hip(hipDeviceSynchronize());
    std::cout << "Kernel launched from .cpp file!" << std::endl;
    std::cout << "Value c[10] = " << c[10] <<" = " << a[10] << " + " << b[10] << std::endl;
    std::cin.get();
    hip_tutorials::check_hip(hipFree(d_a));
    hip_tutorials::check_hip(hipFree(d_b));
    hip_tutorials::check_hip(hipFree(d_c));
    return 0;
}