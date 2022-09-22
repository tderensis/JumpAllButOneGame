add_executable (jump-all-but-one-game main.cpp)

include(FetchContent)

FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 9.1.0)

FetchContent_MakeAvailable(fmt)

target_include_directories(jump-all-but-one-game PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(jump-all-but-one-game PRIVATE fmt::fmt)
