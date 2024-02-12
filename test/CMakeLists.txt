# CTestを読み込む
include(CTest)

# テスト登録関数
function(register_test)
	cmake_parse_arguments(
        REG_TEST
        ""
        "Name"
        "Sources Targets" ${ARGN})
    add_executable(${REG_TEST_Name})
    target_sources(${REG_TEST_Name} PRIVATE "${REG_TEST_Name}.cpp" ${REG_TEST_Sources})
    target_include_directories(${REG_TEST_Name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${LibName} PRIVATE ${REG_TEST_Targets})
    add_test(Name ${REG_TEST_Name} COMMAND ${REG_TEST_Name})
endfunction(register_test)

# テストを追加する
register_test(Name TestCsmCodec TARGETS CompScancodeMap)
