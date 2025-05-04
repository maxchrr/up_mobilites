cmake_minimum_required(VERSION 3.15) # FetchContent is available in 3.11+
project(up_mobilites)
set(CMAKE_C_STANDARD 99) # Requires C99 standard
set(MY_C_FLAGS ${CMAKE_C_FLAGS} "-O0" "-Wall" "-Wextra" "-pedantic" "-Wno-c11-extensions" "-g")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding Raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
  endif()
endif()

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/sources/*.c") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/sources/") # Define PROJECT_INCLUDE to be the path to the include directory of the project

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_compile_options(${PROJECT_NAME} PRIVATE ${MY_C_FLAGS})

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/vendor/") # Set the asset path macro to the absolute path on the dev machine

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# Tests
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/tests/*.c") # Define TEST_SOURCES as a list of all test files

# Declaring our executable
set(TEST_NAME test_sort)
add_executable(${TEST_NAME}
    ${TEST_SOURCES}
    "${CMAKE_CURRENT_LIST_DIR}/sources/sort.c"
    "${CMAKE_CURRENT_LIST_DIR}/sources/api.c"
    "${CMAKE_CURRENT_LIST_DIR}/sources/utils.c"
    "${CMAKE_CURRENT_LIST_DIR}/sources/list.c"
)
target_sources(${TEST_NAME} PRIVATE ${TEST_SOURCES})
target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_INCLUDE})
target_compile_options(${TEST_NAME} PRIVATE ${MY_C_FLAGS})
