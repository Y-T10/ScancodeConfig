# 引数をパースする
add_library(ModImGui)

# リンクディレクトリに追加
target_include_directories(ModImGui PUBLIC
    "${IMGUI_SOURCE_DIR}/backends/"
    "${IMGUI_SOURCE_DIR}/"
)

# ターゲットのビルド情報を設定する
target_sources(ModImGui PRIVATE
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_link_libraries(ModImGui PUBLIC SDL3::SDL3)