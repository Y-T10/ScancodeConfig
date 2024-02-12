# このライブラリディレクトリを追加する
add_exe_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    LIB_PRI fmt::fmt Threads::Threads challenger::challenger
)
