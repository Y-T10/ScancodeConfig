#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <format>

#include "CprocExec.hpp"

TEST_CASE("Execute as Admin") {
#ifdef _WIN32
    const auto handle = CmpProc::ExecElevated(true, "C:/Windows/notepad.exe");

    if (!handle) {
        MESSAGE(std::format("error code: {}", handle.error()));
        if (handle.error() == ERROR_CANCELLED) {
            MESSAGE("管理者での実行がキャンセルされました");
            return;
        }
        CHECK(false);
        return;
    }
    WaitForSingleObject(handle.value().get(), INFINITE);
#endif
};