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

    const std::expected<DWORD, DWORD> WaitUntilExit(const object_handle& handle) noexcept {
        // プログラムの終了を待つ
        WaitForSingleObject(handle.get(), INFINITE);

        // プログラムの終了コードを取得する．
        if (DWORD code = 0; GetExitCodeProcess(handle.get(), &code) != 0) {
            return std::expected<DWORD, DWORD>(code);
        }
        return std::unexpected(GetLastError());
    }
}