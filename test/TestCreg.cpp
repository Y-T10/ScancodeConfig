#include <algorithm>
#include <cstdint>
#include <vector>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "CregHandler.hpp"
#include "CregAdmin.hpp"

using namespace CompReg;

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

TEST_CASE("Check Privilege") {
    if(const auto result = IsElevated();!result || !(*result)) {
        MESSAGE("このテストを行う際は管理者権限が必要です．");
        return;
    }

    CHECK(EnableWritingRegPriv());
    const auto writeKeySuccess = OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
        KEY_SET_VALUE
    );
    CHECK(writeKeySuccess);
    DisableWritingRegPriv();
};