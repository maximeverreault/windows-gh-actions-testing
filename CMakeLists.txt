cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(windows-gh-actions-testing LANGUAGES C CXX)

add_library(${PROJECT_NAME} main.cpp)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)

target_compile_definitions(${PROJECT_NAME} PUBLIC DISABLE_MCAL_FILE_VERSION_CHECK)


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME}_Tests)
set_target_properties(${PROJECT_NAME}_Tests PROPERTIES CXX_STANDARD 20)
target_link_libraries(
        ${PROJECT_NAME}_Tests
        GTest::gtest_main
        ${PROJECT_NAME}
)
add_subdirectory(tests)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Tests XML_OUTPUT_DIR report DISCOVERY_MODE PRE_TEST)