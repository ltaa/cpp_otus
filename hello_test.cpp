#define BOOST_TEST_MODULE test_string
#include <boost/test/included/unit_test.hpp>
#include "hello.h"
#include <string>
using namespace boost::unit_test;

//BOOST_AUTO_TEST_SUITE(test_string)

BOOST_AUTO_TEST_CASE(test_func_string) {

    BOOST_CHECK(printHello("world!") == std::string("Hello, world!"));

}
//}

