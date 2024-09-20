# CTestを読み込む
include(CTest)
# doctestを読み込む
find_package(doctest REQUIRED)

# Ctestヘルパーモジュールを読み込む
include(${CMAKE_SOURCE_DIR}/cmake/CtestHelper.cmake)

# テストを追加する
register_test(TestCsmCodec Target CompScancodeMap doctest::doctest)
register_test(TestKeyName Target CompScancodeMap)
register_test(TestCreg Target CompRegistry doctest::doctest)
register_test(TestProc Target CompProcess doctest::doctest)
# doctestに追加する
doctest_discover_tests(TestCsmCodec)
