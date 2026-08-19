#ifndef OPS_H
#define OPS_H

namespace cuda_tutorials {
    extern "C" {
        void launch_vector_add(const float* a, const float* b, float* c, int n);
    }
}

#endif
