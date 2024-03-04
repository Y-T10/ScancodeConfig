#include <algorithm>
#include <cstdint>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "CregHandler.hpp"
#include "CregAdmin.hpp"

using namespace CompReg;

#ifdef _WIN32
const auto OpenRegKbLayout(const REGSAM access) noexcept {
    return OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"),
        access
    );
}
#endif

void TestRegWriteInternal(const std::vector<uint8_t>& value) noexcept {
#ifdef _WIN32
    constexpr auto ValueName = TEXT("Scancode Map");

    // キーを生成する．パスのキーが存在しない場合にも対応する．
    const auto ReadKey = OpenRegKbLayout(KEY_READ);
    const auto WriteKey = OpenRegKbLayout(KEY_SET_VALUE);
    CHECK(ReadKey);
    CHECK(WriteKey);

    // 既存の値を待避する
    const auto ValOrigin = ReadKeyValueBin(ReadKey, ValueName);
    CHECK(ValOrigin);

    // データを書き込む
    CHECK(WriteKeyValueBin(WriteKey, ValueName, value));
    // 値を取り出す．
    const auto readValue = ReadKeyValueBin(ReadKey, ValueName);
    CHECK(readValue);
    // 読んだ値と書いた値が一致しているかを調べる
    CHECK(std::ranges::equal(*readValue, value));

    // 既存のデータを元に戻す
    CHECK(WriteKeyValueBin(WriteKey, ValueName, *ValOrigin));
#endif
}

TEST_CASE("Open registry key") {
#ifdef _WIN32
    const auto readKey = OpenRegKbLayout(KEY_READ);
    CHECK(readKey);

    const auto result = IsElevated();
    CHECK(result);

    const auto writeKey = OpenRegKbLayout(KEY_WRITE);
    CHECK((!!writeKey) == *result);
#endif
};

TEST_CASE("Read Registry Value") {
#ifdef _WIN32
    const auto readKey = OpenRegKbLayout(KEY_READ);
    CHECK(readKey);

    // 値の読み込み
    const auto value = ReadKeyValueBin(readKey, TEXT("Scancode Map"));
    CHECK(value);
#endif
};

#include "TestBinData.hpp"

TEST_CASE("Write Scancode Map") {
#ifdef _WIN32
    if(const auto result = IsElevated();!result || !(*result)) {
        MESSAGE("このテストを行う際は管理者権限が必要です．");
        return;
    }

    TestRegWriteInternal(EmptyMap);
    TestRegWriteInternal(ExaMap1);
    TestRegWriteInternal(ExaMap2);
#endif
};