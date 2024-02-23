block(SCOPE_FOR POLICIES)
function(set_target_build_internal TargetName)
    # 引数をパースする
	cmake_parse_arguments(
        Param "" "Debug" "SOURCE;LIB_PUB;LIB_PRI" ${ARGN}
    )
    
    if(NOT (DEFINED Param_SOURCE))
        message(FATAL_ERROR "ソースファイルが指定されていません．")
        return()
    endif()

    ## ターゲットの定義ディレクトリを取得
    get_target_property(target_dir ${TargetName} SOURCE_DIR)

    target_sources(${TargetName} PRIVATE ${Param_SOURCE})
    target_include_directories(${TargetName} PUBLIC ${target_dir})
    target_link_libraries(${TargetName} PUBLIC ${Param_LIB_PUB})
    target_link_libraries(${TargetName} PRIVATE ${Param_LIB_PRI})
    target_compile_options(${TargetName} PRIVATE $<$<BOOL:${Param_Debug}>:-g3>)
endfunction()

function(add_lib_directory Directory)
    cmake_parse_arguments(
        Param "Debug" "" "SOURCE;LIB_PUB;LIB_PRI" ${ARGN}
    )
    get_filename_component(Name ${Directory} NAME_WE)
    add_library(${Name})
    set_target_build_internal(
        ${Name}
        Debug ${Param_Debug}
        SOURCE ${Param_SOURCE}
        LIB_PUB ${Param_LIB_PUB}
        LIB_PRI ${Param_LIB_PRI}
    )
endfunction(add_lib_directory)

function(add_exe_directory Directory)
    cmake_parse_arguments(
        Param "Debug" "" "SOURCE;LIB_PUB;LIB_PRI" ${ARGN}
    )
    get_filename_component(Name ${Directory} NAME_WE)
    add_executable(${Name})
    set_target_build_internal(
        ${Name} 
        Debug ${Param_Debug}
        SOURCE ${Param_SOURCE}
        LIB_PUB ${Param_LIB_PUB}
        LIB_PRI ${Param_LIB_PRI}
    )
endfunction(add_exe_directory)
endblock()
