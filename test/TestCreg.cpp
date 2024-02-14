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
    CHECK(readKey != nullptr);

    const auto result = IsElevated();
    CHECK(result.has_value());

    const auto writeKey = OpenRegKey(
        HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout"), 
        KEY_WRITE
    );
    CHECK(*result);
    CHECK((writeKey != nullptr) == *result);
};