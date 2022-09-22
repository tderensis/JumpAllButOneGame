
include(CTest)

FetchContent_Declare(ut
    GIT_REPOSITORY https://github.com/boost-ext/ut.git
    GIT_TAG v1.1.9)

FetchContent_MakeAvailable(ut)

add_executable(board_tests board_tests.cpp)
target_include_directories(board_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(board_tests PRIVATE Boost::ut)

add_test(NAME board_tests
         COMMAND board_tests)
