cmake_minimum_required(VERSION 3.10)
project(git_user_tracker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(APPINDICATOR REQUIRED ayatana-appindicator3-0.1)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
pkg_check_modules(CAIRO REQUIRED cairo)

# If CMAKE_BUILD_TYPE variable is not set, it defaults to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# If CMAKE_BUILD_TYPE is set to "Debug", it adds the 'DEBUG_MODE' preprocessor to the compiler definitions.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG_MODE)
endif()

add_executable(
    git_user_tracker 
    src/app.cpp
    src/cmd.cpp
    src/config.cpp
    src/icon.cpp
    src/main.cpp
)

target_link_libraries(git_user_tracker 
    ${GTK3_LIBRARIES}
    ${APPINDICATOR_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${CAIRO_LIBRARIES}
)

target_include_directories(git_user_tracker PRIVATE
    ${GTK3_INCLUDE_DIRS}
    ${APPINDICATOR_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    src
)

# Compile options
target_compile_options(git_user_tracker PRIVATE
    ${GTK3_CFLAGS_OTHER}
    ${APPINDICATOR_CFLAGS_OTHER}
    ${JSONCPP_CFLAGS_OTHER}
    ${CAIRO_CFLAGS_OTHER}
    # -g  # Add debug symbols
    -Wall
    -Wextra
    -Wpedantic
)

# Installation
install(TARGETS git_user_tracker DESTINATION bin)
