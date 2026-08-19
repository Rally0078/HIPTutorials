# GPU Tutorials (CUDA & HIP)

This repository contains GPU programming tutorials written using the **CUDA API**, with dual-backend support for both **NVIDIA CUDA** and **AMD ROCm / HIP** via CMake.

---

## Features

- **Dual-Backend Support**: Switch seamlessly between CUDA and HIP backends using the `GPU_BACKEND` CMake option.
- **CUDA Native Codebase**: Kernels and applications are written in CUDA C++ syntax. When building for HIP (`GPU_BACKEND=HIP`), header macros automatically translate CUDA types and runtime calls (`cudaMalloc`, `cudaMemcpy`, `cudaGetDeviceProperties`, etc.) to HIP equivalents (`hipMalloc`, `hipMemcpy`, `hipGetDeviceProperties`, etc.).

---

## Building the Project

### Option A: NVIDIA CUDA Backend (Default)

#### Prerequisites
- NVIDIA CUDA Toolkit (e.g. CUDA 12.x / 13.x)
- Visual Studio 2022 with C++ Workload (provides `cl.exe` host compiler)

#### Build Commands

```powershell
# 1. Configure CMake targeting your GPU architecture (e.g., 86 for RTX 30-series)
cmake -B build_cuda -S . -G "Visual Studio 17 2022" -A x64 -DGPU_BACKEND=CUDA -DCMAKE_CUDA_ARCHITECTURES=86

# 2. Build Release binaries
cmake --build build_cuda --config Release

# 3. Run an experiment executable
.\build_cuda\Release\01_firstrun_add.exe
```

---

### Option B: AMD ROCm / HIP Backend

#### Prerequisites
- AMD ROCm SDK (e.g. ROCm 6.4 installed at `C:/Program Files/AMD/ROCm/6.4`)
- Ninja build generator and AMD Clang compilers

#### Build Commands

```powershell
# 1. Configure CMake with the HIP backend and target AMD GPU architecture (e.g., gfx1201 for RX 9070 XT, gfx1100 for RX 7900 XTX)
cmake -B build_hip -S . -G "Ninja" `
  -DGPU_BACKEND=HIP `
  -DCMAKE_HIP_ARCHITECTURES="gfx1201" `
  -DCMAKE_C_COMPILER="C:/Program Files/AMD/ROCm/6.4/bin/clang.exe" `
  -DCMAKE_CXX_COMPILER="C:/Program Files/AMD/ROCm/6.4/bin/clang++.exe" `
  -DCMAKE_HIP_COMPILER="C:/Program Files/AMD/ROCm/6.4/bin/clang++.exe" `
  -DCMAKE_NINJA_FORCE_RESPONSE_FILE=ON `
  -DCMAKE_CXX_SCAN_FOR_MODULES=OFF

# 2. Build the project executables
cmake --build build_hip

# 3. Run an experiment executable
.\build_hip\01_firstrun_add.exe
```

---

## CLion Setup

Target include directories for both CUDA Toolkit and ROCm are automatically exported to CMake targets (`gpu_kernels` and `experiment_base`).

If opening in **CLion**:
1. Open the project folder in CLion.
2. Select **Tools -> CMake -> Reload CMake Project**.
3. CLion's `clangd` linter will automatically resolve CUDA (`<cuda_runtime.h>`) and HIP (`<hip/hip_runtime.h>`) system headers.