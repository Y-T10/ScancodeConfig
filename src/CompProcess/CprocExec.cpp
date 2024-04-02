#include "CprocExec.hpp"

namespace CmpProc {
    const std::expected<object_handle, DWORD> ExecElevated(bool isShow, const CompReg::win32str& execPath, const CompReg::win32str& param) noexcept {
        const SHELLEXECUTEINFO info = {
            .cbSize = sizeof(SHELLEXECUTEINFO),
            .fMask = SEE_MASK_DEFAULT | SEE_MASK_NOCLOSEPROCESS,
            .hwnd = NULL,
            .lpVerb = TEXT("runas"),
            .lpFile = execPath.c_str(),
            .lpParameters = param.empty()? nullptr: param.c_str(),
            .lpDirectory = nullptr,
            .nShow = isShow? SW_SHOW: SW_HIDE,
            .hInstApp = nullptr,
            .lpIDList = nullptr,
            .lpClass = nullptr,
            .hkeyClass = nullptr,
            .dwHotKey = 0,
            .hProcess = nullptr
        };

        if (ShellExecuteEx(const_cast<SHELLEXECUTEINFO*>(&info))) {
            return object_handle(info.hProcess);
        }
        return std::unexpected(GetLastError());
    }
}