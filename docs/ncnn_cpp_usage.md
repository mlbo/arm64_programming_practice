# ncnn C++ 使用指南

## 目录

- [简介](#简介)
- [构建 ncnn](#构建-ncnn)
- [集成到项目](#集成到项目)
- [基础 API 使用](#基础-api-使用)
- [模型加载方式](#模型加载方式)
- [推理流程](#推理流程)
- [操作系统兼容性处理](#操作系统兼容性处理)
- [性能优化](#性能优化)
- [常见问题与解决方案](#常见问题与解决方案)

---

## 简介

ncnn 是腾讯开源的高性能神经网络前向计算框架，专为移动端优化。特点：

- **无第三方依赖**：核心库不依赖任何第三方库
- **跨平台**：支持 Linux、Windows、macOS、Android、iOS、WebAssembly 等
- **CPU/GPU 加速**：支持 CPU 多线程和 Vulkan GPU 加速
- **低内存占用**：针对移动设备优化
- **支持量化**：支持 INT8 量化推理

---

## 构建 ncnn

### 下载源码

```bash
git clone https://github.com/Tencent/ncnn.git
cd ncnn
git submodule update --init
```

### Linux 构建

```bash
# 安装依赖
sudo apt install build-essential git cmake libprotobuf-dev protobuf-compiler libomp-dev libopencv-dev

# 构建
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DNCNN_VULKAN=ON -DNCNN_BUILD_EXAMPLES=ON ..
make -j$(nproc)
make install
```

### macOS 构建

```bash
# 使用 Homebrew 安装
brew install ncnn

# 或手动编译
brew install protobuf libomp
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DNCNN_VULKAN=ON -DNCNN_BUILD_EXAMPLES=ON ..
make -j$(sysctl -n hw.ncpu)
make install
```

### Windows 构建 (Visual Studio)

```cmd
# 安装 protobuf 后
mkdir build && cd build
cmake -A x64 -DCMAKE_INSTALL_PREFIX=%cd%/install -Dprotobuf_DIR=<protobuf-path>/cmake -DNCNN_VULKAN=ON ..
cmake --build . --config Release -j 4
cmake --build . --config Release --target install
```

### Android 构建

```bash
export ANDROID_NDK=<your-ndk-path>

# arm64-v8a
mkdir build-android-aarch64 && cd build-android-aarch64
cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-21 \
    -DNCNN_VULKAN=ON ..
make -j$(nproc)
make install
```

### iOS 构建

```bash
mkdir build-ios && cd build-ios
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/ios.toolchain.cmake \
    -DPLATFORM=OS64 \
    -DARCHS="arm64;arm64e" \
    -DENABLE_BITCODE=0 \
    -DNCNN_VULKAN=ON ..
cmake --build . -j 4
cmake --build . --target install
```

### WebAssembly 构建

```bash
# 安装 Emscripten
source emsdk_env.sh

mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
    -DNCNN_THREADS=OFF -DNCNN_OPENMP=OFF \
    -DNCNN_RUNTIME_CPU=OFF -DNCNN_SSE2=OFF \
    -DNCNN_BUILD_TOOLS=OFF -DNCNN_BUILD_EXAMPLES=OFF ..
cmake --build . -j 4
```

---

## 集成到项目

### 方式一：CMake find_package（推荐）

```cmake
# 设置 ncnnConfig.cmake 所在目录
set(ncnn_DIR "<ncnn_install_dir>/lib/cmake/ncnn" CACHE PATH "Directory that contains ncnnConfig.cmake")

find_package(ncnn REQUIRED)
target_link_libraries(your_target ncnn)
```

### 方式二：手动链接

```cmake
# Linux/macOS
target_include_directories(your_target PRIVATE <ncnn_install_dir>/include)
target_link_libraries(your_target
    <ncnn_install_dir>/lib/libncnn.a
    <ncnn_install_dir>/lib/libglslang.a  # 如果启用 Vulkan
)

# Windows (Visual Studio)
# Debug 模式
target_link_libraries(your_target
    <ncnn_install_dir>/lib/ncnnd.lib
    <ncnn_install_dir>/lib/glslangd.lib
)
# Release 模式
target_link_libraries(your_target
    <ncnn_install_dir>/lib/ncnn.lib
    <ncnn_install_dir>/lib/glslang.lib
)
```

### Android 项目集成

```cmake
# CMakeLists.txt
set(ncnn_DIR ${CMAKE_SOURCE_DIR}/sdk/${ANDROID_ABI}/lib/cmake/ncnn)
find_package(ncnn REQUIRED)

add_library(your_lib SHARED your_code.cpp)
target_link_libraries(your_lib ncnn)
```

---

## 基础 API 使用

### C++ API 示例

```cpp
#include "net.h"

int main() {
    // 1. 创建网络
    ncnn::Net net;

    // 2. 设置选项（必须在加载模型前）
    net.opt.use_vulkan_compute = true;  // 启用 GPU 加速
    net.opt.num_threads = 4;            // CPU 线程数

    // 3. 加载模型
    if (net.load_param("model.param") != 0) return -1;
    if (net.load_model("model.bin") != 0) return -1;

    // 4. 创建输入 Mat
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(
        image_data,           // 图像数据指针
        ncnn::Mat::PIXEL_BGR, // 输入格式
        img_w, img_h,         // 原始尺寸
        target_w, target_h    // 目标尺寸
    );

    // 5. 归一化
    const float mean_vals[3] = {104.f, 117.f, 123.f};
    const float norm_vals[3] = {1.f, 1.f, 1.f};
    in.substract_mean_normalize(mean_vals, norm_vals);

    // 6. 创建提取器并推理
    ncnn::Extractor ex = net.create_extractor();
    ex.input("input_blob_name", in);

    ncnn::Mat out;
    ex.extract("output_blob_name", out);

    // 7. 处理输出
    for (int i = 0; i < out.w; i++) {
        float value = out[i];
        // ...
    }

    return 0;
}
```

### C API 示例

```c
#include "c_api.h"

int main() {
    // 创建网络和选项
    ncnn_net_t net = ncnn_net_create();
    ncnn_option_t opt = ncnn_option_create();

    ncnn_option_set_use_vulkan_compute(opt, 1);
    ncnn_option_set_num_threads(opt, 4);
    ncnn_net_set_option(net, opt);

    // 加载模型
    if (ncnn_net_load_param(net, "model.param") != 0) return -1;
    if (ncnn_net_load_model(net, "model.bin") != 0) return -1;

    // 创建输入
    ncnn_mat_t in = ncnn_mat_from_pixels_resize(
        image_data, NCNN_MAT_PIXEL_BGR,
        img_w, img_h, img_w * 3,
        target_w, target_h, NULL
    );

    const float mean_vals[3] = {104.f, 117.f, 123.f};
    ncnn_mat_substract_mean_normalize(in, mean_vals, NULL);

    // 推理
    ncnn_extractor_t ex = ncnn_extractor_create(net);
    ncnn_extractor_input(ex, "input_blob_name", in);

    ncnn_mat_t out;
    ncnn_extractor_extract(ex, "output_blob_name", &out);

    // 获取输出
    const float* data = (const float*)ncnn_mat_get_data(out);
    int out_w = ncnn_mat_get_w(out);

    // 清理资源
    ncnn_mat_destroy(in);
    ncnn_mat_destroy(out);
    ncnn_extractor_destroy(ex);
    ncnn_option_destroy(opt);
    ncnn_net_destroy(net);

    return 0;
}
```

---

## 模型加载方式

### 加载 API 汇总表

| 加载来源 | alexnet.param | alexnet.param.bin | alexnet.bin |
|---------|---------------|-------------------|-------------|
| 文件路径 | `load_param(const char*)` | `load_param_bin(const char*)` | `load_model(const char*)` |
| Windows wchar_t | `load_param(const wchar_t*)` | `load_param_bin(const wchar_t*)` | `load_model(const wchar_t*)` |
| 文件描述符 | `load_param(FILE*)` | `load_param_bin(FILE*)` | `load_model(FILE*)` |
| 内存 | `load_param_mem(const char*)` | `load_param(const unsigned char*)` | `load_model(const unsigned char*)` |
| Android Asset | `load_param(AAsset*)` | `load_param_bin(AAsset*)` | `load_model(AAsset*)` |
| Android Asset 路径 | `load_param(AAssetManager*, const char*)` | `load_param_bin(AAssetManager*, const char*)` | `load_model(AAssetManager*, const char*)` |
| 自定义 Reader | `load_param(const DataReader&)` | `load_param_bin(const DataReader&)` | `load_model(const DataReader&)` |

### 模型文件组合

以下两种组合任选其一：

1. `alexnet.param` + `alexnet.bin`
2. `alexnet.param.bin` + `alexnet.bin`

### Android Asset 加载（推荐）

```cpp
#include <android/asset_manager.h>

// 从 Asset 直接加载，避免复制到 SD 卡
AAssetManager* mgr = ...; // 从 Java 获取

ncnn::Net net;
net.load_param(mgr, "models/model.param");
net.load_model(mgr, "models/model.bin");
```

### 内存加载（适用于模型加密）

```cpp
// 从内存加载，可用于模型解密后加载
std::vector<unsigned char> param_data = load_and_decrypt("model.param.bin");
std::vector<unsigned char> model_data = load_and_decrypt("model.bin");

ncnn::Net net;
net.load_param_bin(param_data.data());
net.load_model(model_data.data());
```

### 模型内嵌到代码

使用 `ncnn2mem` 工具将模型转换为 C++ 代码：

```bash
./ncnn2mem model.param model.bin model.id.h model.mem.h
```

生成的文件可直接编译进程序：

```cpp
#include "model.id.h"
#include "model.mem.h"

ncnn::Net net;
net.load_param_bin(model_param_bin);
net.load_model(model_bin);

// 使用枚举而非字符串名称
ex.input(INPUT_DATA, in);
ex.extract(OUTPUT_PROB, out);
```

---

## 推理流程

### 完整推理示例

```cpp
int run_inference(const cv::Mat& image, std::vector<float>& results) {
    // 1. 创建网络
    ncnn::Net net;

    // 2. 配置选项
    net.opt.num_threads = 4;
    net.opt.use_vulkan_compute = true;    // GPU 加速
    net.opt.use_fp16_storage = true;      // FP16 存储
    net.opt.use_fp16_arithmetic = false;  // FP16 计算（可能损失精度）

    // 3. 加载模型
    net.load_param("model.param");
    net.load_model("model.bin");

    // 4. 预处理图像
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(
        image.data,
        ncnn::Mat::PIXEL_BGR2RGB,  // 可选通道转换
        image.cols, image.rows,
        640, 480
    );

    // 均值和标准化
    const float mean[3] = {123.675f, 116.28f, 103.53f};
    const float std[3] = {58.395f, 57.12f, 57.375f};
    in.substract_mean_normalize(mean, std);

    // 5. 推理
    ncnn::Extractor ex = net.create_extractor();
    ex.set_light_mode(true);  // 轻量模式，节省内存
    ex.input("input", in);

    ncnn::Mat out;
    ex.extract("output", out);

    // 6. 后处理
    results.resize(out.w * out.h * out.c);
    for (int i = 0; i < out.total(); i++) {
        results[i] = out[i];
    }

    return 0;
}
```

### 多输入多输出

```cpp
ncnn::Extractor ex = net.create_extractor();

// 多个输入
ex.input("input1", in1);
ex.input("input2", in2);

// 多个输出
ncnn::Mat out1, out2;
ex.extract("output1", out1);
ex.extract("output2", out2);
```

### Vulkan GPU 推理

```cpp
ncnn::Net net;

// 设置 GPU 设备
net.set_vulkan_device(0);  // 使用第一个 GPU

// 或使用 VulkanDevice 句柄
// net.set_vulkan_device(vkdev);

net.opt.use_vulkan_compute = true;
net.load_param("model.param");
net.load_model("model.bin");

// 推理会自动在 GPU 上执行
ncnn::Extractor ex = net.create_extractor();
ex.input("data", in);
ex.extract("prob", out);
```

---

## 操作系统兼容性处理

### 问题一：老旧操作系统 C++ 支持不完整

#### 解决方案 1：禁用 RTTI 和异常

```bash
cmake -DNCNN_DISABLE_RTTI=ON -DNCNN_DISABLE_EXCEPTION=ON ..
```

```cpp
// 代码中不能使用 dynamic_cast 和 try-catch
// 使用返回值判断错误
```

#### 解决方案 2：使用 ncnn 内置 STL 替代

```bash
cmake -DNCNN_SIMPLESTL=ON ..
```

编译时添加参数：
```bash
-nodefaultlibs -fno-builtin -nostdinc++ -lc
```

### 问题二：Windows XP 支持

```bash
# 使用 MinGW
cmake -DCMAKE_TOOLCHAIN_FILE="../toolchains/windows-xp-mingw.toolchain.cmake" \
    -DNCNN_WINXP=ON \
    -DNCNN_SIMPLEOCV=ON \
    -DNCNN_AVX=OFF \
    -G "MinGW Makefiles" ..

# 使用 Visual Studio (v141_xp 工具集)
cmake -A WIN32 -G "Visual Studio 17 2022" -T v141_xp \
    -DNCNN_WINXP=ON \
    -DNCNN_SIMPLEOCV=ON \
    -DNCNN_OPENMP=OFF \
    -DNCNN_AVX=OFF \
    -DCMAKE_TOOLCHAIN_FILE="../toolchains/windows-xp-msvc.toolchain.cmake" ..
```

### 问题三：嵌入式系统（无标准 C++ 库）

```bash
# 禁用所有依赖
cmake -DNCNN_DISABLE_RTTI=ON \
    -DNCNN_DISABLE_EXCEPTION=ON \
    -DNCNN_SIMPLESTL=ON \
    -DNCNN_STDIO=OFF \
    -DNCNN_STRING=OFF \
    -DNCNN_VULKAN=OFF \
    -DNCNN_OPENMP=OFF \
    -DNCNN_PIXEL=OFF \
    ..

# 禁用特定指令集
cmake -DNCNN_RUNTIME_CPU=OFF \
    -DNCNN_AVX=OFF \
    -DNCNN_AVX2=OFF \
    -DNCNN_ARM82=OFF \
    ..
```

### 问题四：Android OpenMP 问题

**问题**：`dlopen failed: library "libomp.so" not found`

**解决方案**：

```cmake
# 链接静态 OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstatic-openmp -fopenmp")
```

**问题**：crash with `__kmp_abort_process`

**解决方案**：

```cmake
# 方案 1：旧版 NDK
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic -lomp -Wl,-Bdynamic")

# 方案 2：NDK >= 21
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstatic-openmp")
```

### 问题五：iOS Bitcode 问题

```bash
# 禁用 Bitcode（默认）
cmake -DENABLE_BITCODE=0 ..

# 启用 Bitcode（App Store 需要）
cmake -DENABLE_BITCODE=1 ..
```

### 问题六：WebAssembly 平台限制

```bash
# 基础版本（最大兼容性）
cmake -DNCNN_THREADS=OFF \
    -DNCNN_OPENMP=OFF \
    -DNCNN_RUNTIME_CPU=OFF \
    -DNCNN_SSE2=OFF \
    -DNCNN_AVX2=OFF \
    -DNCNN_AVX=OFF \
    ..

# 启用 SIMD 扩展
cmake -DNCNN_SSE2=ON ..

# 启用线程扩展
cmake -DNCNN_THREADS=ON -DNCNN_OPENMP=ON -DNCNN_SIMPLEOMP=ON ..
```

### 问题七：交叉编译

ncnn 提供了丰富的工具链文件：

| 目标平台 | 工具链文件 |
|---------|-----------|
| ARM Cortex-A | `arm-linux-gnueabi.toolchain.cmake` |
| AArch64 | `aarch64-linux-gnu.toolchain.cmake` |
| Android | NDK 自带 `android.toolchain.cmake` |
| iOS | `ios.toolchain.cmake` |
| Raspberry Pi | `pi3.toolchain.cmake` |
| Nvidia Jetson | `jetson.toolchain.cmake` |
| RISC-V | `riscv64-unknown-linux-gnu.toolchain.cmake` |
| HiSilicon | `hisiv300.toolchain.cmake` 等 |
| QNX | `aarch64-qnx.toolchain.cmake` |
| ESP32 | `esp32.toolchain.cmake` |

使用示例：

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/aarch64-linux-gnu.toolchain.cmake ..
```

---

## 性能优化

### CPU 优化

```cpp
ncnn::Net net;

// 线程数（通常设为物理核心数）
net.opt.num_threads = 4;

// 大小核绑定
ncnn::set_cpu_powersave(2);  // 0=全部, 1=小核, 2=大核

// FP16 存储（减少内存占用）
net.opt.use_fp16_storage = true;

// BF16 存储（ARM A53 等）
net.opt.use_bf16_storage = true;

// Packing layout（SIMD 优化）
net.opt.use_packing_layout = true;
```

### GPU 优化

```cpp
net.opt.use_vulkan_compute = true;

// 使用 FP16 存储（GPU 友好）
net.opt.use_fp16_storage = true;

// 查询 GPU 数量
int gpu_count = ncnn::get_gpu_count();

// 选择 GPU
net.set_vulkan_device(0);
```

### 内存优化

```cpp
// 轻量模式（释放中间层内存）
ex.set_light_mode(true);

// 自定义内存分配器
ncnn::Allocator* allocator = ncnn::create_pool_allocator();
ex.set_blob_allocator(allocator);
```

### 库体积优化

```bash
# 禁用未使用的功能
cmake -DNCNN_VULKAN=OFF \
    -DNCNN_OPENMP=OFF \
    -DNCNN_STDIO=OFF \
    -DNCNN_STRING=OFF \
    -DNCNN_BF16=OFF \
    -DNCNN_INT8=OFF \
    -DNCNN_PIXEL=OFF \
    -DNCNN_PIXEL_DRAWING=OFF \
    -DNCNN_RUNTIME_CPU=OFF \
    ..

# 禁用未使用的层
cmake -DWITH_LAYER_absval=OFF -DWITH_LAYER_bnll=OFF ..
```

### 模型优化

使用 `ncnnoptimize` 优化模型：

```bash
# FP16 优化
./ncnnoptimize model.param model.bin opt.param opt.bin 65536

# FP32 优化
./ncnnoptimize model.param model.bin opt.param opt.bin 0

# 裁剪模型
./ncnnoptimize model.param model.bin opt.param opt.bin 0 start_layer end_layer
```

### INT8 量化

```bash
# 生成量化表
./ncnn2table model.param model.bin images.txt model.table

# 量化模型
./ncnn2int8 model.param model.bin quantized.param quantized.bin model.table
```

---

## 常见问题与解决方案

### 问题：`parse magic failed`

**原因**：模型文件格式过旧或损坏

**解决**：使用最新 ncnn 工具重新转换模型

### 问题：`layer XYZ not exists or registered`

**原因**：模型包含 ncnn 不支持的层

**解决**：
1. 实现自定义层
2. 或注册为空操作层：

```cpp
class Noop : public ncnn::Layer {};
DEFINE_LAYER_CREATOR(Noop)

net.register_custom_layer("CustomLayerName", Noop_layer_creator);
```

### 问题：`network graph not ready`

**原因**：未正确加载模型

**解决**：确保先调用 `load_param()` 再调用 `load_model()`，并检查返回值

### 问题：`set_vulkan_compute failed`

**原因**：在加载模型后设置 Vulkan

**解决**：在 `load_param()` 前设置 `net.opt.use_vulkan_compute = true`

### 问题：GPU 占用为 0

**解决**：
- Windows：任务管理器 → 性能 → GPU → 切换到 Compute 视图
- 使用 GPU-Z 工具监控

### 问题：结果不正确

**检查清单**：
1. 输入预处理是否正确（均值、标准化、通道顺序）
2. 输入尺寸是否匹配
3. 禁用 FP16 测试：`net.opt.use_fp16_storage = false`

### 问题：如何获取中间层输出

```cpp
ncnn::Extractor ex = net.create_extractor();
ex.input("input", in);

ncnn::Mat intermediate;
ex.extract("intermediate_blob_name", intermediate);
```

### 问题：如何打印 ncnn::Mat

```cpp
void pretty_print(const ncnn::Mat& m) {
    for (int q = 0; q < m.c; q++) {
        const float* ptr = m.channel(q);
        for (int y = 0; y < m.h; y++) {
            for (int x = 0; x < m.w; x++) {
                printf("%f ", ptr[x]);
            }
            ptr += m.w;
            printf("\n");
        }
        printf("------------------------\n");
    }
}
```

### 问题：RTTI/异常冲突

```bash
# 重新编译 ncnn，匹配项目设置
cmake -DNCNN_DISABLE_RTTI=ON -DNCNN_DISABLE_EXCEPTION=ON ..
# 或
cmake -DNCNN_DISABLE_RTTI=OFF -DNCNN_DISABLE_EXCEPTION=OFF ..
```

---

## 参考资源

- [ncnn GitHub](https://github.com/Tencent/ncnn)
- [ncnn Wiki](https://github.com/Tencent/ncnn/wiki)
- [模型转换工具 PNNX](https://github.com/pnnx/pnnx)
- [预训练模型](https://github.com/nihui/ncnn-assets)
- [问题反馈](https://github.com/Tencent/ncnn/issues)