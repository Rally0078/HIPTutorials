#ifndef KERNELS_H
#define KERNELS_H
#include <iostream>
#include <source_location>
namespace hip_tutorials {

    inline void check_hip(hipError_t status,
                         const std::source_location loc = std::source_location::current()) {
        if (status != hipSuccess) {
            std::cerr << "HIP Error: " << hipGetErrorString(status) << "\n"
                      << "Location: " << loc.file_name() << ":" << loc.line() << "\n"
                      << "Function: " << loc.function_name() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}
#endif