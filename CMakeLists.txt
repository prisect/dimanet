cmake_minimum_required(VERSION 3.16)
project(dimanet VERSION 3.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wshadow -Wstrict-prototypes
                        -Wmissing-prototypes -Wpointer-arith -pedantic)
endif()

include(GNUInstallDirs)

add_library(dimanet STATIC dimanet.c)
target_include_directories(dimanet PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(dimanet PUBLIC m)

add_library(dimanet_shared SHARED dimanet.c)
set_target_properties(dimanet_shared PROPERTIES OUTPUT_NAME dimanet)
target_include_directories(dimanet_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(dimanet_shared PUBLIC m)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
enable_testing()

add_executable(dimanet_test tests/dimanet_test.c dimanet.c)
target_include_directories(dimanet_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(dimanet_test PRIVATE m)
add_test(NAME dimanet_test COMMAND dimanet_test)

# ---------------------------------------------------------------------------
# Examples
# ---------------------------------------------------------------------------
foreach(n 1 2 3 4 5)
    add_executable(example_${n} examples/example_${n}.c dimanet.c)
    target_include_directories(example_${n} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(example_${n} PRIVATE m)
endforeach()

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install(TARGETS dimanet dimanet_shared
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES dimanet.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
