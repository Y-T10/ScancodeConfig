#include "CprocPipe.hpp"

#include <array>
#include <cstdint>
#include <iterator>
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

    const std::expected<object_handle, DWORD> CreatePipe(const CompReg::win32str& name, const size_t bufferSize) noexcept {
        const DWORD PipeMode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE |  PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS;
        const auto Handle = CreateNamedPipe(
            name.c_str(),
            PIPE_ACCESS_OUTBOUND,
            PipeMode,
            1,
            bufferSize,
            bufferSize,
            0,
            NULL
        );

        if(Handle == INVALID_HANDLE_VALUE) {
            return std::unexpected(GetLastError());
        }
        return object_handle(const_cast<HANDLE>(Handle));
    }

    const std::expected<void, DWORD> WritePipe(const object_handle& handle, const std::vector<uint8_t>& byte) noexcept {
        DWORD writtenSize = 0;
        if (WriteFile(handle.get(), byte.data(), byte.size(), &writtenSize, nullptr)) {
            return std::expected<void, DWORD>();
        }
        return std::unexpected(GetLastError());
    }

    const std::expected<std::vector<uint8_t>, DWORD> ReadPipe(const object_handle& handle) noexcept {
        std::vector<uint8_t> data = {};

        for(DWORD readSize = 0; true; readSize = 0) {
            auto buff = std::array<uint8_t, 256>{};       
            if (!ReadFile(handle.get(), buff.data(), buff.size(), &readSize, nullptr)) {
                return std::unexpected{GetLastError()};
            }
            if (readSize == 0) {
                break;
            }
            std::copy(buff.begin(), buff.begin() + readSize, std::back_inserter(data));
            // TODO: 何度も読めるようにしたい
            break;
        }

        return data;
    }
}