# My HIP Tutorial

I made this repo to learn HIP and GPU programming in general. 
My environment is Windows, and the CMakeLists.txt has a hardcoded Windows path.

## Prerequisites:
The CMake flags to add in Windows are: 
```bash
-DCMAKE_C_COMPILER="C:/Program Files/AMD/ROCm/6.4/bin/clang.exe" 
-DCMAKE_CXX_COMPILER="C:/Program Files/AMD/ROCm/6.4/bin/clang++.exe" 
-DCMAKE_HIP_COMPILER="C:/Program Files/AMD/ROCm/6.4/bin/clang++.exe" 
-DCMAKE_NINJA_FORCE_RESPONSE_FILE=ON 
-DCMAKE_CXX_SCAN_FOR_MODULES=OFF
```

Maybe none of this is required in Linux?

## Running
Just build the appropriate executable from Experiments to get the executable using CMake.