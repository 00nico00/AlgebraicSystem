# 分析代数系统(S*)的性质和类型

此为离散数学二实验代码

# 编译运行
```bash
git clone https://github.com/00nico00/AlgebraicSystem.git
```
接着删除 `CMakeLists.txt` 中的：
```cmake
set(TEST-SOURCES
        test/main.cpp
)

add_executable(Test ${HEADERS} ${TEST-SOURCES})

find_package(GTest CONFIG REQUIRED)
target_link_libraries(Test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
```

## 使用 IDE 集成 cmake编译
使用对于 IDE （CLion 或 Visual Studio）打开编译即可

## 使用 CMake 命令行编译
```bash
mkdir build
cd build
cmake ..
cmake --build build
./AlgebraicSystem.exe
```