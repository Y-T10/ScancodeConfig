# このライブラリディレクトリを追加する
add_exe_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMappingWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfRegistry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMappingIO.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfDialog.cpp
    LIBPRIV
        CompRegistry
        CompScancodeMap
        challenger::challenger
        ModImGui
        msgpack-cxx
        CompProcess
)
