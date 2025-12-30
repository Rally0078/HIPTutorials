#ifndef OPS_H
#define OPS_H
namespace hip_tutorials {
    // These functions use C-linkage but are tucked inside a C++ namespace
    extern "C" {
        void launch_vector_add(const float* a, const float* b, float* c, int n);
    }
}
#endif
