#include "SDL_render.h"
#include "SDL_video.h"
#include "challenger/challenger_memory.hpp"
#include "challenger/challenger_video.hpp"
#include "challenger/challenger_render.hpp"
#include <cstddef>
#include <cstdlib>

using namespace challenger;

int main(int argc, char* argv[]) {
    // ウィンドウとレンダラを作成
    const auto MainWindow = Create<Window, SDL_CreateWindow>("Scancode Configure", 300, 300, SDL_WINDOW_OPENGL);
    const auto WindowRenderer = Create<Renderer, SDL_CreateRenderer>(MainWindow.get(), nullptr, SDL_RENDERER_ACCELERATED);

    return EXIT_SUCCESS;
}