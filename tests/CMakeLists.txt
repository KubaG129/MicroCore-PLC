include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 #
)

FetchContent_MakeAvailable(Catch2)


set(TEST_SOURCES
    test_main.cpp
    test_memory.cpp
)


add_executable(PlcTests ${TEST_SOURCES})

target_link_libraries(PlcTests PRIVATE Catch2::Catch2WithMain)
target_include_directories(PlcTests PRIVATE ../src)