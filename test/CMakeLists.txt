include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.0-preview4
)
FetchContent_MakeAvailable(Catch2)

add_executable(Tests ${TESTS})
target_include_directories(Tests PRIVATE ../include)
target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(Tests)
