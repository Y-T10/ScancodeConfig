# このライブラリディレクトリを追加する
add_executable(AppScancodeConf-qt WIN32)
set_target_properties(AppScancodeConf-qt
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
)

# ターゲットのビルド情報を設定する
target_include_directories(AppScancodeConf-qt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(AppScancodeConf-qt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMapModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AsconfAddDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AsconfMappingTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(AppScancodeConf-qt PRIVATE
    CompRegistry
    CompScancodeMap
    Qt6::Widgets
    msgpack-cxx
    CompProcess
    Threads::Threads
)
qt_disable_unicode_defines(AppScancodeConf-qt)
