cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(webapi  VERSION ${PROJECT_VESRION})

add_executable(webapi webapi.cpp)

set_target_properties(webapi PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

set(MASTER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")

install(TARGETS webapi RUNTIME DESTINATION bin)


# lots of warnings and all warnings as errors
target_compile_options(webapi PRIVATE -Wall -Wextra -pedantic -Werror -g)


# add_subdirectory(gtests)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
