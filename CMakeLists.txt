cmake_minimum_required(VERSION 3.9)

project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Threads REQUIRED)

add_library(async SHARED src/async.cpp src/async.h)

add_executable(main_test main.cpp)

target_link_libraries(async ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(main_test async)

set_target_properties(main_test async PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-fsanitize=thread"
    LINK_FLAGS "-fsanitize=thread"
)

install(TARGETS async LIBRARY DESTINATION lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT jacksonrr3@yandex.ru)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
