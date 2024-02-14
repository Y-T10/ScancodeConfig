#include "CregAdmin.hpp"
#include <memory>
#include <minwindef.h>
#include <optional>
#include <processthreadsapi.h>
#include <type_traits>

#ifndef STRICT
#define STRICT
#endif
#ifndef NOMIMAX
#define NOMIMAX
#endif
#include <windows.h>
#include <winnt.h>

namespace CompReg {
    const std::optional<bool> IsElevated() noexcept {
        using Handle = std::unique_ptr<
            std::remove_pointer_t<HANDLE>,
            decltype([](HANDLE ptr){ CloseHandle(ptr); })
            >;

        Handle token = nullptr;
        if (HANDLE h = nullptr; OpenProcessToken(GetCurrentProcess(), TOKEN_READ, &h) == 0) {
            return std::nullopt;
        } else {
            token = Handle(h);
        }

        TOKEN_ELEVATION evelationInfo = {};
        DWORD size = sizeof(evelationInfo);
        if (0 == GetTokenInformation(token.get(), TOKEN_INFORMATION_CLASS::TokenElevation, &evelationInfo, size, &size)) {
            return std::nullopt;
        } 
        return std::make_optional(!!evelationInfo.TokenIsElevated);
    }
}