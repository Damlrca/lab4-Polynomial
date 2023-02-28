add_executable(test test_main.cpp test_Polynomial.cpp)

target_include_directories(test PUBLIC ../gtest)
target_link_libraries(test gtest)

target_include_directories(test PUBLIC ../Polynomial)
target_link_libraries(test Polynomial)
