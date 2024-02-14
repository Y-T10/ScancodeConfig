#pragma once

#ifndef STRICT
#define STRICT
#endif
#ifndef NOMIMAX
#define NOMIMAX
#endif
#include "windows.h"

namespace CompReg {
    /**
     * @brief 管理者権限が利用できるかを調べる
     * @return true 利用可能
     * @return false 利用不可能
     */
    const bool IsElevated() noexcept;
}