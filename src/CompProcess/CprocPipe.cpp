#include "CprocPipe.hpp"

#include <array>
#include <random>
#include <algorithm>

namespace {
    const char8_t ToAcceptedChar(const char8_t c) noexcept {
        // 文字列を、' '(\)を除く'!'(0x21)から'~'(0x7f)までの文字列に置き換える．

        const auto ReplaceBackSlash = [](const char8_t c) {
            return c == u8'\\'? u8'[': c;
        };

        const auto RplaceOutOfRangeChar = [](const char8_t c) -> char8_t {
            // 文字を'~'以下にする
            const auto SingleChar = (c % 127);
            // 文字を' '以上にする
            return SingleChar <= u8' '?
                SingleChar + 33:
                SingleChar;
        };

        return ReplaceBackSlash(RplaceOutOfRangeChar(c));
    }
}

namespace CmpProc {
    const CompReg::win32str CreateRandomPipeName() noexcept {
        // パイプ名のプレフィックス
        const CompReg::win32str Prefix = TEXT("\\\\.\\pipe\\");

        // パイプ名の本体生成関数
        const auto RandomName = []() -> const CompReg::win32str {
        std::random_device Randseed;
        std::mt19937_64 Rand(Randseed());

            std::array<uint64_t, 32> origin = {};
            std::generate(origin.begin(), origin.end(), Rand);

            std::array<uint8_t, 256> chars = {};
            std::ranges::fill(chars, '\0');
            memcpy(chars.data(), origin.data(), chars.size());
            std::transform(chars.begin(), chars.end(), chars.begin(), ToAcceptedChar);

            return CompReg::win32str(chars.begin(), chars.end());
        };

        // 名前を作成
        const auto Name = RandomName();

        // プレフィックスと名前を結合
        return Prefix + Name.substr(0, Name.size() - Prefix.size() - 1);
    }
}