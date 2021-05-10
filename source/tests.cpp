//
// Created by Pamula File on 5/9/21.
//
#include "../header.h"

void    tests(std::list<server> conf)
{
    std::cout << "root: " << (*((*conf.begin()).get_routes().begin())).get_root()  << std::endl;
    std::pair<std::string, std::string>     p;
    try {
        p = (*conf.begin()).split_request("http://yandex.ru/docs/index.php?method=GET,name=spoils");
    }
    catch (std::exception &e)
    {
        std::cout << "no request!"  << std::endl;
    }
    std::cout << p.first << " " << p.second  << std::endl;
}