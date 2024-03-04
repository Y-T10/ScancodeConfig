#include "CregAdmin.hpp"

#include <type_traits>
#include <memory>
#include "CregWinCommonHeader.hpp"

#ifdef _WIN32
#include <minwindef.h>
#include <processthreadsapi.h>
#include <securitybaseapi.h>
#endif

namespace {
#ifdef _WIN32
    using HToken = std::unique_ptr<
        std::remove_pointer_t<HANDLE>,
        decltype([](HANDLE ptr){ CloseHandle(ptr); })
    >;

    const HToken OpenCurrenProcToken(const DWORD access) noexcept {
        HANDLE h = nullptr;
        if (!OpenProcessToken(GetCurrentProcess(), access, &h)) {
            return nullptr;
        }
        return HToken(h);
    }

    template<bool isEnable>
    const bool SetTokenPriv(const HToken& token, const TCHAR* privName){
        LUID privLUID = {};

        if (!LookupPrivilegeValue(NULL, privName, &privLUID)) {
            return false;
        }

        const TOKEN_PRIVILEGES TokePriv = {
            .PrivilegeCount = 1,
            .Privileges = {
                LUID_AND_ATTRIBUTES{
                    .Luid = privLUID,
                    .Attributes = isEnable? SE_PRIVILEGE_ENABLED: 0
                }
            }
        };

        // トークンの特権を設定し、その結果を返す
        if (!AdjustTokenPrivileges(
                token.get(), FALSE, 
                (TOKEN_PRIVILEGES*)&TokePriv, sizeof(TOKEN_PRIVILEGES),
                NULL, NULL)) {
                return false;
            }
        return GetLastError() == ERROR_SUCCESS;
    };
#endif
}

namespace CompReg {
    #ifdef _WIN32
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
    #endif
}