block(SCOPE_FOR POLICIES)
# テスト登録関数
function(register_test Name)
	cmake_parse_arguments(
        param "" "" "Source;Target" ${ARGN}
    )

    add_executable(${Name})
    target_sources(${Name} PRIVATE "${Name}.cpp" ${param_Source})
    get_target_property(target_dir ${Name} SOURCE_DIR)
    target_include_directories(${Name} PUBLIC ${target_dir})
    target_link_libraries(${Name} PRIVATE ${param_Target})

    add_test(Name ${Name} COMMAND ${Name})
endfunction(register_test)
endblock()
