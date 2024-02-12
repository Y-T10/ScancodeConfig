#include "CsmCodec.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <cstring>
#include <bit>
#include <type_traits>

namespace {
    template <std::integral integer_type>
    const integer_type Byteswap(const integer_type value) noexcept {
        static_assert(std::is_unsigned_v<integer_type>);
        if constexpr (std::is_same_v<integer_type, uint8_t>) { 
            return value;
        } else if constexpr (std::is_same_v<integer_type, uint16_t>) {
            return __builtin_bswap16(value);
        } else if constexpr (std::is_same_v<integer_type, uint32_t>) {
            return __builtin_bswap32(value);
        } else if constexpr (std::is_same_v<integer_type, uint64_t>) {
            return __builtin_bswap64(value);
        } else {
            static_assert(false, "this integer type is not supported.");
        }
    }

    template<class output_type, size_t N = sizeof(output_type)>
    const output_type LittleByte2Int(const uint8_t bin[N]) noexcept {
        output_type output;
        // little endian から 実行時のエンディアンに変換する
        if constexpr (std::endian::native == std::endian::little) {
            mempcpy(&output, bin, sizeof(uint8_t)*N);
        } else if constexpr (std::endian::native == std::endian::big) {
            const auto reverse = std::array<uint8_t, N>(bin, bin + N);
            mempcpy(&output, reverse.data(), reverse.size());
        } else {
            static_assert(false, "this endian is not supported.");
        }
        return output;
    }

    const bool IsVailedScanMapBin(const std::vector<uint8_t>& bin) noexcept {
        const auto isZero = [](const uint8_t value) -> bool {
            return value == 0x00;
        };

        // version, flags の領域が0で埋まっているかを調べる
        if(!std::all_of(bin.begin(), bin.begin() + 8, isZero)) {
            return false;
        }
        // 変換表サイズの領域が0で埋まっているかを調べる
        if(std::any_of(bin.begin() + 8, bin.begin() + 12, isZero)){
            return false;
        }
        // 表の末端が0かを調べる
        return std::any_of(bin.end() - 4, bin.end(), isZero);
    }
}

namespace CompScanMap {
    const std::optional<ConvertMap> DecodeScancodeMap(const std::vector<uint8_t>& bin) noexcept {
        using namespace CompScanMap;

        // 無効なバイナリかを調べる
        if (!IsVailedScanMapBin(bin)) {
            return std::nullopt;
        }

        // 変換表のサイズを得る
        const uint32_t mapSize = LittleByte2Int<uint32_t>(bin.data() + 8);

        // 変換表が空かを調べる
        if (mapSize == 1) {
            return {};
        }

        // 表内の対応ペアの数
        const uint32_t numOfPair = mapSize - 1;

        // 変換表を読み取る
        ConvertMap map = {};
        map.reserve(numOfPair);
        // 変換表の終端ペア (0x00000000)
        const uint8_t* end = (bin.data() + numOfPair*sizeof(ConvertMap::value_type));
        for(const uint8_t* pair = bin.data() + 12; pair != end; pair+=sizeof(ConvertMap::value_type)) {
            map.emplace_back(ConvertPair{
                .from = LittleByte2Int<uint16_t>(pair + 2),
                .to = LittleByte2Int<uint16_t>(pair)
            });
        };

        return map;
    }
}