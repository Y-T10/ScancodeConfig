cmake_minimum_required(VERSION 4.0)

# このプロジェクトの概要を設定する
project(ScancodeConf
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "`Scancode Map`を設定するためのGUIアプリ"
)

# C++コンパイラに渡すオプションを設定する
add_compile_options(-Wall $<$<CONFIG:Debug>:-g3>)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AddTargetHelper.cmake)

# FetchContentを有効にする
include(FetchContent)

# CPMを用いてネット上のパッケージを取り込む
set(MSGPACK_CXX20 ON)
FetchContent_Declare(
    msgpack-cxx
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
    GIT_TAG cpp-7.0.0
)

FetchContent_Declare(
    challenger
    GIT_REPOSITORY https://github.com/Y-T10/challenger.git
    GIT_TAG v0.0.3
)

FetchContent_Declare(
    IMGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.8
)

FetchContent_Declare(
    Fcpp
    GIT_REPOSITORY https://github.com/Y-T10/Fcpp.git
    GIT_TAG main
)

FetchContent_MakeAvailable(msgpack-cxx challenger IMGUI Fcpp)

# システム内のパッケージを読み込む
find_package(Threads REQUIRED)
#find_package(Boost REQUIRED)

# Add sub directories
add_subdirectory(src)

## テストを実施するかを判断する
if(${TEST_IS_ENABLED})
    ## テストを有効にする
    enable_testing()
    ## テストディレクトリ
    add_subdirectory(test)
endif()
