block(SCOPE_FOR POLICIES)
function(set_target_build_internal TargetName)
    # 引数をパースする
    set(_opt "Debug")
    set(_varg SOURCE LIBPRIV LIBPUB)
	cmake_parse_arguments(Param "${_opt}" "" "${_varg}" ${ARGN})
    
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

function(__add_obj_internal Directory EXE)
    get_filename_component(Name ${Directory} NAME_WE)
    if(${EXE})
        add_executable(${Name})
        set_target_properties(${Name}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            ${CMAKE_SOURCE_DIR}/bin
        )
    else()
        add_library(${Name})
    endif()
    set_target_build_internal(${Name} ${ARGN})
endfunction()

## ライブラリターゲットを追加する
function(add_lib_directory Directory)
    __add_obj_internal(${Directory} FALSE ${ARGN})
endfunction(add_lib_directory)

## バイナリターゲットを追加する
function(add_exe_directory Directory)
    __add_obj_internal(${Directory} TRUE ${ARGN})
endfunction(add_exe_directory)

## インターフェイスターゲットを追加する
function(add_interface_directory Directory)
    set(_inter_opt Debug)
    set(_inter_varg LIBPRIV LIBPUB)
    cmake_parse_arguments(arg "${_inter_opt}" "" "${_inter_varg}" ${ARGN})

    ## 新しいターゲットを追加する
    get_filename_component(Name ${arg_Directory} NAME_WE)
    add_library(${Name} INTERFACE)
    ## ターゲットのビルド引数を設定する
    target_include_directories(${Name} INTERFACE ${Directory})
    target_link_libraries(${Name} INTERFACE ${arg_LIBPUB})
    target_link_libraries(${Name} INTERFACE ${arg_LIBPRIV})
    target_compile_options(${Name} INTERFACE $<$<BOOL:${arg_Debug}>:-g3>)
endfunction(add_interface_directory)
endblock()
