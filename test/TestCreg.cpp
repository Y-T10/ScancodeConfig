#include <algorithm>
#include <cstdint>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "CregHandler.hpp"
#include "CregAdmin.hpp"

using namespace CompReg;

void TestRegWriteInternal(const std::vector<uint8_t>& value) noexcept {
    constexpr auto Subkey = TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout");
    constexpr auto ValueName = TEXT("Scancode Map");

    // キーを生成する．パスのキーが存在しない場合にも対応する．
    const auto ReadKey = OpenRegKey(
        HKEY_LOCAL_MACHINE, Subkey, KEY_READ
    );
    const auto WriteKey = OpenRegKey(
        HKEY_LOCAL_MACHINE, Subkey, KEY_SET_VALUE
    );
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
}

TEST_CASE("Open registry key") {
    const auto readKey = OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
        KEY_READ
    );
    CHECK(readKey.get() != nullptr);

    const auto result = IsElevated();
    CHECK(result.has_value());

    const auto writeKey = OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
        KEY_WRITE
    );
    CHECK((writeKey != nullptr) == *result);
};

TEST_CASE("Read Registry Value") {
    const auto readKey = OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
        KEY_READ
    );
    CHECK(readKey != nullptr);

    // 値の読み込み
    const auto value = ReadKeyValueBin(readKey, TEXT("Scancode Map"));
    CHECK(value.has_value());
};

#include "TestBinData.hpp"

TEST_CASE("Write Scancode Map") {
    if(const auto result = IsElevated();!result || !(*result)) {
        MESSAGE("このテストを行う際は管理者権限が必要です．");
        return;
    }

    TestRegWriteInternal(EmptyMap);
    TestRegWriteInternal(ExaMap1);
    TestRegWriteInternal(ExaMap2);
};