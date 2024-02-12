#include "CsmCodec.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <cstring>
#include <bit>
#include <type_traits>

namespace {
    template <class T>
    concept uintegral = std::integral<T> && std::is_unsigned_v<T>;

    template <uintegral integer_type>
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
        output_type output = 0;
        mempcpy(&output, bin, sizeof(uint8_t)*N);
        // little endian から 実行時のエンディアンに変換する
        if constexpr (std::endian::native == std::endian::little) {
            return output;
        } else if constexpr (std::endian::native == std::endian::big) {
            return Byteswap(output);
        } else {
            static_assert(false, "this endian is not supported.");
        }
    }

    /// 実行時バイト順の値をリトルエンディアンの値に変換する．
    template<uintegral integer_type>
    const auto NativeToLittle(const integer_type value) noexcept {
        if constexpr (std::endian::native == std::endian::little) {
            return value;
        } else if constexpr (std::endian::native == std::endian::big) {
            return Byteswap(value);
        } else {
            static_assert(false, "this endian is not supported.");
        }
    }

    /**
     * @brief `Scancode Map`のバイナリに値を書き込む
     * @tparam integer_type 書き込む符号なし整数の型
     * @param bin 書き込み先バイナリのポインタ
     * @param value 書き込む値．実行時のバイト順序で表現されている．
     */
    template<uintegral integer_type>
    inline void WriteScanMapBin(uint8_t* bin, const integer_type value) noexcept {
        const integer_type little_end_value = NativeToLittle(value);
        mempcpy(bin, &little_end_value, sizeof(integer_type));
    }

    /**
     * @brief `Scancode Map`のバイナリに値を書き込む
     * @tparam integer_type 書き込む符号なし整数の型
     * @param bin 書き込み先バイナリのポインタ
     * @param value 書き込む値．実行時のバイト順序で表現されている．
     */
    template<uintegral integer_type>
    inline const integer_type ReadScanMapBin(const uint8_t* bin) noexcept {
        integer_type little_end_value = 0;
        mempcpy(&little_end_value, bin, sizeof(integer_type));
        if constexpr (std::endian::native == std::endian::little) {
            return little_end_value;
        } else if constexpr (std::endian::native == std::endian::big) {
            return Byteswap(little_end_value);
        } else {
            static_assert(false, "this endian is not supported.");
        }
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
        const size_t   headerSize = 12;

        // 変換表を読み取る
        ConvertMap map = ConvertMap(numOfPair, ConvertPair{.to = 0, .from = 0});
        mempcpy(map.data(), bin.data() + headerSize, map.size() * sizeof(map[0]));

        // 実行環境のエンディアンがビックなら値のビット列を反転させる．
        if constexpr (std::endian::native == std::endian::big) {
            std::transform(map.begin(), map.end(), map.begin(), [](const ConvertPair& origin){
                return ConvertPair{.to = Byteswap(origin.to), .from = Byteswap(origin.from)};
            });
            return map;
        } else {
            return map;
        }
    }

    const std::optional<std::vector<uint8_t>> EncodeScancodeMap(const ConvertMap& map) noexcept {
        // 表が空かを調べる
        if (map.empty()) {
            /// 表が空の`Scancode Map`バイナリを返す
            return std::vector<uint8_t> {
                0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
                0x01, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00,
            };
        }

        // 表を`Scancode Map`に格納できないかを調べる
        if (map.size() > (std::numeric_limits<uint32_t>::max() - 1)) {
            return std::nullopt;
        }

        // バイナリ列を生成する
        const size_t BinarySize = 16 + map.size() * sizeof(ConvertMap::value_type);
        std::vector<uint8_t> scanMapBin = std::vector<uint8_t>(BinarySize, 0);
        // 表の要素数を書き込む
        WriteScanMapBin(scanMapBin.data() + 8, map.size() + 1);
        // 表の中身を書き込む
        mempcpy(scanMapBin.data() + 12, map.data(), map.size() * sizeof(map[0]));
        // 実行環境のエンディアンがビックならバイナリ列のスキャンコード値を反転させる．
        if constexpr (std::endian::native == std::endian::big) {
            for(auto valHead = (scanMapBin.begin() + 12); valHead != scanMapBin.end(); valHead+=sizeof(Scancode)) {
                auto valTail = valHead + 1;
                std::swap(*valHead, *valTail);
            }
            return scanMapBin;
        } else {
            return scanMapBin;
        }
    }
}