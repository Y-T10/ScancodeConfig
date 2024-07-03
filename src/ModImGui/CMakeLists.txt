# 引数をパースする
add_library(ModImGui)

# リンクディレクトリに追加
target_include_directories(ModImGui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    "${imgui_SOURCE_DIR}/"
)

# ターゲットのビルド情報を設定する
target_sources(ModImGui PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_sdlrenderer3.cpp
)

target_link_libraries(ModImGui PUBLIC SDL3::SDL3)