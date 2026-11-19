# 1945

Vertical scrolling shoot'em up in C with [raylib](https://www.raylib.com), AIV Videogame Programming, 3rd year

## Build

Development and build on Windows (x64) only, with CMake. raylib 6.2 is already in the repository and is
linked dynamically: headers in `include/`, import library `lib/raylibdll.lib` and `lib/raylib.dll`.

After every build, CMake copies `raylib.dll` and `assets\` next to `game1945.exe`.

### CLion

Open the project folder: CLion picks up `CMakeLists.txt` and builds into `cmake-build-*\` (ignored by git).
In *Settings > Build, Execution, Deployment > Toolchains* use the **Visual Studio** toolchain
(Visual Studio Build Tools + Windows SDK) as the default one, because `lib/raylibdll.lib` is an MSVC
import library and does not work with the bundled MinGW. To compile with clang, set the C compiler of
that toolchain to `C:\Program Files\LLVM\bin\clang.exe`.
Then build and run with the CLion *Run* button.