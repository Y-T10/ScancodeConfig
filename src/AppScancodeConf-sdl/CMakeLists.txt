# このライブラリディレクトリを追加する
add_exe_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMappingWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfRegistry.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMappingIO.cpp
    LIBPRIV
        CompRegistry
        CompScancodeMap
        challenger::challenger
        msgpack-cxx
        CompProcess
        Fcpp::Fcpp
        TGUI::TGUI
)
