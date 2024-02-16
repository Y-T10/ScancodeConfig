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

namespace {
    using HToken = std::unique_ptr<
        std::remove_pointer_t<HANDLE>,
        decltype([](HANDLE ptr){ CloseHandle(ptr); })
    >;

    const HToken OpenCurrenProcToken(const DWORD access) noexcept {
        HANDLE h = nullptr;
        if (!!OpenProcessToken(GetCurrentProcess(), access, &h)) {
            return nullptr;
        }
        return HToken(h);
    }
}

namespace CompReg {
    const std::optional<bool> IsElevated() noexcept {
        const HToken token = OpenCurrenProcToken(TOKEN_READ);
        if(!token) {
            return std::nullopt;
        }

        TOKEN_ELEVATION evelationInfo = {};
        DWORD size = sizeof(evelationInfo);
        if (0 == GetTokenInformation(token.get(), TOKEN_INFORMATION_CLASS::TokenElevation, &evelationInfo, size, &size)) {
            return std::nullopt;
        } 
        return std::make_optional(!!evelationInfo.TokenIsElevated);
    }
}