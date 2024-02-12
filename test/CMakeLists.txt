# CTestを読み込む
include(CTest)
# Ctestヘルパーモジュールを読み込む
include(${CMAKE_SOURCE_DIR}/cmake/CtestHelper.cmake)

# テストを追加する
register_test(TestCsmCodec Target CompScancodeMap)
