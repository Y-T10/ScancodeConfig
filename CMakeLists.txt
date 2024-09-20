cmake_minimum_required(VERSION 3.20)

# このプロジェクトの概要を設定する
project(ScancodeConf
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "`Scancode Map`を設定するためのGUIアプリ"
)

# ビルド周りの設定
option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(CMAKE_BUILD_TYPE "Build type." Release)

# CMAKE_EXPORT_COMPILE_COMMANDSの既定値を設定する
if ((NOT (DEFINED $ENV{CMAKE_EXPORT_COMPILE_COMMANDS})) AND (DEFINED CMAKE_EXPORT_COMPILE_COMMANDS))
  set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

# C++コンパイラに渡すオプションを設定する
add_compile_options(-Wall $<$<CONFIG:Debug>:-g3>)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 23)

# CPMをダウンロードする
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DownloadFile.cmake)
DownloadFile(
    "https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.7/CPM.cmake"
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake
    83e5eb71b2bbb8b1f2ad38f1950287a057624e385c238f6087f94cdfc44af9c5
)

# CPMを有効にする
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/AddTargetHelper.cmake)

# FetchContentを有効にする
include(FetchContent)

# CPMを用いてネット上のパッケージを取り込む
set(MSGPACK_CXX20 ON)
FetchContent_Declare(
    msgpack-cxx
    GIT_REPOSITORY https://github.com/msgpack/msgpack-c.git
    GIT_TAG cpp-6.1.0
)

FetchContent_Declare(
    challenger
    GIT_REPOSITORY https://github.com/Y-T10/challenger.git
    GIT_TAG v0.0.1
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

## 親プロジェクトが存在するかを調べる
if(NOT (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME}))
    ## ここで処理を終わらせる
    return()
endif()

## テストを有効にする
enable_testing()
## テストディレクトリ
add_subdirectory(test)
