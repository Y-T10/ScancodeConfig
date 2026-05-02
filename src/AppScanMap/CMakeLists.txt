add_executable(scancodemap)
target_sources(scancodemap PRIVATE main.cpp)
target_link_libraries(scancodemap PRIVATE CompRegistry CompScancodeMap CompProcess)
