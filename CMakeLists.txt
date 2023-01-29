cmake_minimum_required(VERSION 3.20)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(windows-gh-actions-testing LANGUAGES C CXX)

add_library(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set_target_properties(${PROJECT_NAME} PROPERTIES C_STANDARD 11)

target_compile_definitions(${PROJECT_NAME} PUBLIC DISABLE_MCAL_FILE_VERSION_CHECK)

add_subdirectory(vxlapi)
target_link_libraries(${PROJECT_NAME} PRIVATE vxlapi)

add_subdirectory(stubs)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
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
        GTest::gmock_main
        ${PROJECT_NAME}
        vxlapi
)
add_subdirectory(tests)
add_subdirectory(mocks)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Tests XML_OUTPUT_DIR report)