add_executable(Sample_exe main.cpp)

target_include_directories(Sample_exe PUBLIC ../Polynomial)
target_link_libraries(Sample_exe Polynomial)
