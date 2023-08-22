cmake_minimum_required(VERSION 3.14)

set(Test_Files
    "test_glass.cpp"
    "test_provider.cpp"
    "test_state.cpp"
)

add_executable(sorter_unit_tests ${Test_Files})

target_link_libraries(sorter_unit_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(sorter_unit_tests)