add_executable(Test1 "test1.cpp" "String_helpers.h") # "new_Print.h" "new_Printable.h")
set_property(TARGET Test1 PROPERTY CMAKE_CXX_STANDARD 14)

add_executable(Test2 "test2.cpp")
set_property(TARGET Test2 PROPERTY CMAKE_CXX_STANDARD 14)

add_executable(dtest "dtest_main.cpp" "String_helpers.h" "new_String.h")
set_property(TARGET dtest PROPERTY CXX_STANDARD 14)
#set_property(TARGET dtest PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(dtest PRIVATE doctest::doctest)
doctest_discover_tests(dtest)

