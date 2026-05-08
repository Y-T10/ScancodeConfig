add_executable(scancodemap)
target_sources(scancodemap PRIVATE main.cpp)
set_target_properties(scancodemap
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin 
)
target_link_libraries(scancodemap PRIVATE
    CompRegistry
    CompScancodeMap
    CompProcess
    -lstdc++exp
    utf8proc::utf8proc
)
