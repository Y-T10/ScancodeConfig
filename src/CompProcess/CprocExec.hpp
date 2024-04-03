#pragma once

#include "CregTypes.hpp"
#include <expected>
#include "CproHandle.hpp"

namespace CmpProc {
    /**
     * @brief 管理者権限でプログラムを起動する
     * @param execPath プログラムへのパス
     * @return 成功すれば起動したプロセスのハンドル．失敗したらWindowsのエラーコード．
     */
    const std::expected<object_handle, DWORD> ExecElevated(bool isShow, const CompReg::win32str& execPath, const CompReg::win32str& param = "") noexcept;

    /**
     * @brief プロセスが終了するまで待機し、その返値を返す．
     * @param handle プロセスのハンドル
     * @return 成功すればプロセスの返値を、失敗すればエラーコードを返す．
     */
    const std::expected<DWORD, DWORD> WaitUntilExit(const object_handle& handle) noexcept;
}