#define BOOST_TEST_MODULE filter_test
#include <boost/test/included/unit_test.hpp>
#include "filter.h"
#include <string>
#include <iostream>

using namespace boost::unit_test;

struct redirect_output {
    redirect_output(std::streambuf *buf) : old(std::cout.rdbuf(buf)) {}
    ~redirect_output() {
        std::cout.rdbuf(old);
    }
  std::streambuf *old;
};



BOOST_AUTO_TEST_SUITE(filter_testing_suite)


BOOST_AUTO_TEST_CASE(printOne_test_case) {
    boost::test_tools::output_test_stream output_stream;

    redirect_output ro(output_stream.rdbuf());
    std::vector<std::string> source = { "1.2.3.4",
                                        "2.3.4.5",
                                        "2.3.5.7",
                                        "1.1.1.1",
                                        "123.132.145.154",
                                        "12.13.41.52",
                                        "52.53.45.55",
                                        "1.33.213.44" };

    std::vector<std::vector<int>> ip_pool;

    for (const auto &elem : source) {
        auto ip_string = split(elem, '.');
        std::vector<int> ip_elem;
        bool ok;
        std::tie(ip_elem, ok) = str_toip(ip_string);

        ip_pool.push_back(ip_elem);

    }

    std::string expect = std::string("1.2.3.4\n")+
            std::string("1.1.1.1\n") +
            std::string("1.33.213.44\n");

    printIpSequence(ip_pool,1);
    BOOST_CHECK(output_stream.is_equal( expect));
}



BOOST_AUTO_TEST_CASE(printFirstTwo_test_case) {
    boost::test_tools::output_test_stream output_stream;

    redirect_output ro(output_stream.rdbuf());
    std::vector<std::string> source = { "1.2.3.4",
                                        "2.3.4.5",
                                        "2.3.5.7",
                                        "1.2.1.1",
                                        "123.132.145.154",
                                        "12.13.41.52",
                                        "52.53.45.55",
                                        "1.33.213.44" };

    std::vector<std::vector<int>> ip_pool;

    for (const auto &elem : source) {
        auto ip_string = split(elem, '.');
        std::vector<int> ip_elem;
        bool ok;
        std::tie(ip_elem, ok) = str_toip(ip_string);

        ip_pool.push_back(ip_elem);

    }

    std::string expect = std::string("1.2.3.4\n")+
            std::string("1.2.1.1\n");

    printIpSequence(ip_pool,1,2);
    BOOST_CHECK(output_stream.is_equal( expect));
}


BOOST_AUTO_TEST_CASE(printAny_test_case) {
    boost::test_tools::output_test_stream output_stream;

    redirect_output ro(output_stream.rdbuf());
    std::vector<std::string> source = { "1.2.3.4",
                                        "2.3.4.5",
                                        "2.3.5.7",
                                        "1.2.1.1",
                                        "123.132.145.154",
                                        "12.13.41.52",
                                        "52.53.45.55",
                                        "1.33.213.44" };

    std::vector<std::vector<int>> ip_pool;

    for (const auto &elem : source) {
        auto ip_string = split(elem, '.');
        std::vector<int> ip_elem;
        bool ok;
        std::tie(ip_elem, ok) = str_toip(ip_string);

        ip_pool.push_back(ip_elem);

    }

    std::string expect = std::string("1.2.3.4\n") +
            std::string("2.3.4.5\n") +
            std::string("2.3.5.7\n") +
            std::string("1.2.1.1\n");


    printAny(ip_pool,2);
    BOOST_CHECK(output_stream.is_equal( expect));
}


BOOST_AUTO_TEST_SUITE_END()

