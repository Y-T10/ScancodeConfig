#pragma once

#include "CregTypes.hpp"

namespace CmpProc {
    /**
     * @brief パイプの名前を生成する
     * @return パイプ名を持つ文字列
     */
    const CompReg::win32str CreateRandomPipeName() noexcept;
}