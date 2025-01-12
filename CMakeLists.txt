cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(webapi  VERSION ${PROJECT_VESRION})


add_executable(webapi src/webapi.cpp src/WebServer.cpp src/HTTPSession.cpp src/WebsocketSession.cpp src/Listener.cpp)

set_target_properties(webapi PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# lots of warnings and all warnings as errors
target_compile_options(webapi PRIVATE -Wall -Wextra -pedantic -Werror -g)

find_package(boost_algorithm CONFIG REQUIRED)
target_link_libraries(webapi PRIVATE Boost::algorithm)

find_package(boost_json REQUIRED CONFIG)
target_link_libraries(webapi PRIVATE Boost::json)

find_package(boost_asio REQUIRED CONFIG)
target_link_libraries(webapi PRIVATE Boost::asio)

find_package(Boost REQUIRED COMPONENTS beast)
target_link_libraries(webapi PRIVATE Boost::beast)

find_package(boost_property_tree REQUIRED CONFIG)
target_link_libraries(webapi PRIVATE Boost::property_tree)

find_package(boost_process REQUIRED CONFIG)
target_link_libraries(webapi PRIVATE Boost::process)

set(MASTER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/")

install(TARGETS webapi RUNTIME DESTINATION bin)


# add_subdirectory(gtests)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
