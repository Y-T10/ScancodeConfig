# このライブラリディレクトリを追加する
add_exe_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMapModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMainWindow.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfAddDialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMappingTable.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    LIBPRIV
        CompRegistry
        CompScancodeMap
        Qt6::Widgets
        msgpack-cxx
        CompProcess
)

get_filename_component(TargetName ${CMAKE_CURRENT_SOURCE_DIR} NAME_WE)
qt_disable_unicode_defines(${TargetName})
