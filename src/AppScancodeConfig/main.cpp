#include "fmt/core.h"
#include "fmt/printf.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO)){
        return 1;
    }
    fmt::println("sample program with fmt!");
    SDL_Quit();
    return 0;
}