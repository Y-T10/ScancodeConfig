block(SCOPE_FOR POLICIES)
set(_common_varg SOURCE LIBPRIV LIBPUB)
function(set_target_build_internal TargetName)
    # 引数をパースする
    set(_arg Debug)
    set(_varg ${_common_varg})
	cmake_parse_arguments(Param "" "${_arg}" "${_varg}" ${ARGN})
    
    if(NOT (DEFINED Param_SOURCE))
        message(FATAL_ERROR "ソースファイルが指定されていません．")
        return()
    endif()

    # リンクディレクトリに追加
    get_target_property(target_dir ${TargetName} SOURCE_DIR)
    target_include_directories(${TargetName} PUBLIC ${target_dir})
    # ターゲットのビルド情報を設定する
    target_sources(${TargetName} PRIVATE ${Param_SOURCE})
    target_link_libraries(${TargetName} PUBLIC ${Param_LIBPUB})
    target_link_libraries(${TargetName} PRIVATE ${Param_LIBPRIV})
    target_compile_options(${TargetName} PRIVATE $<$<BOOL:${Param_Debug}>:-g3>)
endfunction()

function(__add_obj_internal)
    set(_obj_opt Debug EXE)
    set(_obj_arg Directory)
    set(_obj_varg ${_common_varg})
    cmake_parse_arguments(arg "${_obj_opt}" "${_obj_arg}" "${_obj_varg}" ${ARGN})

    get_filename_component(Name ${arg_Directory} NAME_WE)
    if(${arg_EXE})
        add_executable(${Name})
    else()
        add_library(${Name})
    endif()
    set_target_build_internal(
        ${Name}
        Debug ${arg_Debug}
        SOURCE ${arg_SOURCE}
        LIBPUB ${arg_LIBPUB}
        LIBPRIV ${arg_LIBPRIV}
    )
endfunction()

## ライブラリターゲットを追加する
function(add_lib_directory Directory)
    __add_obj_internal(Directory ${Directory} ${ARGN})
endfunction(add_lib_directory)

## バイナリターゲットを追加する
function(add_exe_directory Directory)
    __add_obj_internal(Directory ${Directory} EXE ${ARGN})
endfunction(add_exe_directory)

## インターフェイスターゲットを追加する
function(add_interface_directory Directory)
    set(_inter_opt Debug)
    set(_inter_varg LIBPRIV LIBPUB)
    cmake_parse_arguments(arg "${_inter_opt}" "${_inter_varg}" ${ARGN})

    ## 新しいターゲットを追加する
    get_filename_component(Name ${arg_Directory} NAME_WE)
    add_library(${Name} INTERFACE)
    ## ターゲットのビルド引数を設定する
    target_include_directories(${Name} INTERFACE ${Directory})
    target_link_libraries(${Name} INTERFACE ${arg_LIBPUB})
    target_link_libraries(${Name} INTERFACE ${arg_LIBPRIV})
    target_compile_options(${Name} INTERFACE $<$<BOOL:${arg_Debug}>:-g3>)
endfunction(add_exe_directory)
endblock()
