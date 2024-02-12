# ディレクトリ名からライブラリ名をとる
get_filename_component(LibName "${CMAKE_CURRENT_SOURCE_DIR}" NAME_WE CACHE)

# ライブラリ名
add_library(${LibName})

# ソースファイルを追加
target_sources(
    ${LibName} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CsmCodec.cpp
)
# ヘッダファイルのディレクトリを追加
target_include_directories(${LibName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# プログラムが利用するターゲットを追加
#target_link_libraries(${LibName} PRIVATE ...)
