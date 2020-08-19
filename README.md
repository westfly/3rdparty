
测试将第三方外链集成到工程中

## submodules ##

```cmake
add_subdirectory(spdlog EXCLUDE_FROM_ALL)

target_link_libraries(
    submodules_test PRIVATE
    spdlog::spdlog
)
```
各种添加即可，引用 spdlog 的链接即可。

需要主动拉取，自动编译spdlog。



## externals ##

借用 External_Project的方式



## vcpkg

vcpkg 是一个管理包，将其作为submodule进行编译。

下载源码，进行编译生成binary，然后安装fmt包

```
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
vcpkg install fmt
```

在工程中引用需要将其添加到顶层的CMakeLists.txt中去
```
set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")
find_package(fmt CONFIG REQUIRED)
target_link_libraries(main PRIVATE fmt::fmt fmt::fmt-header-only)
```

vcpkg 能够很好的解决folly 等知名开源库的编译，真正开箱即用，如果依赖的库有所更新
可以通过
```
vcpkg upgrade
```
自动重新编译


