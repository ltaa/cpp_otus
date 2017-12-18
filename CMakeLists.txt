cmake_minimum_required(VERSION 3.2)

project(ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
add_executable(${PROJECT_NAME} "main.cpp")

add_library(filter "filter.cpp")

add_executable(filter_test "filter_test.cpp")



find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set_target_properties(${PROJECT_NAME} filter filter_test PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wall -Wextra -Wpedantic


    )

set_target_properties(filter_test PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS}
    )


target_link_libraries(filter_test
    filter
    )

target_link_libraries(${PROJECT_NAME}
    filter
    )
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Andrew Teslya") #required
INCLUDE(CPack)

enable_testing()
add_test(filter_test filter_test)
