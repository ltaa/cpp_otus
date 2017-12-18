#include "filter.h"
#include <string>
#include <tuple>

std::vector<std::string> split(const std::__cxx11::string &str, char d)
{
    auto r = std::vector<std::string>();

    std::string::size_type start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

auto str_toip(const std::vector<std::string> &container) -> decltype(std::make_tuple(std::vector<int>(),true)) {
    auto ip =  std::vector<int> ();
    bool ok = false;
    for ( const auto &c : container) {
        try {
            auto ip_part = std::stoi(c);
            ip.push_back(ip_part);

            ok = true;
        } catch (std::invalid_argument &e) {
            std::cout<<e.what()<<std::endl;
        } catch (std::out_of_range e) {
            std::cout<<e.what()<<std::endl;
        }
    }

    return std::make_tuple(ip, ok);
}


