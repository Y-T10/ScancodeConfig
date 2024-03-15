#include "CsmViewer.hpp"
#include "fmt/core.h"

using namespace CompScanMap;

void PrintKeyName(const Scancode code) noexcept {
    const auto Name = CompScanMap::KeyboardKeyName(code);
    fmt::println("{:#06x} {:s}", code, Name.value_or("name_not_found"));
}

int main() {
    for(Scancode i = 0; i <= 0x53; ++i) {
        PrintKeyName(i);
    }
    PrintKeyName(0x0056);
    PrintKeyName(0x0057);
    PrintKeyName(0x0058);

    PrintKeyName(0x0070);
    PrintKeyName(0x0073);
    PrintKeyName(0x0079);
    PrintKeyName(0x007b);
    PrintKeyName(0x007d);
    PrintKeyName(0x007e);

    PrintKeyName(0xE01C); 
    PrintKeyName(0xE01D);
    PrintKeyName(0xE035);
    PrintKeyName(0xE037);
    PrintKeyName(0xE038);
    PrintKeyName(0xE045);
    PrintKeyName(0xE046);
    PrintKeyName(0xE047); 
    PrintKeyName(0xE048);
    PrintKeyName(0xE049);
    PrintKeyName(0xE04B);
    PrintKeyName(0xE04D);
    PrintKeyName(0xE04F);
    PrintKeyName(0xE050);
    PrintKeyName(0xE051);
    PrintKeyName(0xE052);
    PrintKeyName(0xE053);
    PrintKeyName(0xE05B);
    PrintKeyName(0xE05C);
    PrintKeyName(0xE05D);

    return 0;
};