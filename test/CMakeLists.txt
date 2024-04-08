# CTestを読み込む
include(CTest)
## テストフレームワークを追加
CPMAddPackage(
    NAME doctest
    GITHUB_REPOSITORY doctest/doctest
    GIT_TAG v2.4.11
)
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

# Ctestヘルパーモジュールを読み込む
include(${CMAKE_SOURCE_DIR}/cmake/CtestHelper.cmake)

# テストを追加する
register_test(TestCsmCodec Target CompScancodeMap doctest::doctest)
register_test(TestKeyName Target CompScancodeMap)
register_test(TestCreg Target CompRegistry doctest::doctest)
register_test(TestProc Target CompProcess doctest::doctest)
# doctestに追加する
doctest_discover_tests(TestCsmCodec)
