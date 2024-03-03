# このライブラリディレクトリを追加する
add_exe_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AscScanMap.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AscWindowMapping.cpp
    LIBPRIV
        fmt::fmt Threads::Threads PkgConfig::GTK4
        CompScancodeMap
)
