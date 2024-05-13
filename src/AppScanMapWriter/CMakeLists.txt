# このライブラリディレクトリを追加する
add_exe_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    LIBPRIV
        CompRegistry
        CompScancodeMap
        CompProcess
)

target_link_options(
    AppScanMapWriter
    PRIVATE
        -static -static-libgcc -static-libstdc++
)
