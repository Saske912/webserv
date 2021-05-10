//
// Created by Pamula File on 5/9/21.
//
#include "../header.h"

void    tests(std::list<server> conf)
{
    std::string request = "http://yandex.ru/docs/index.php?method=GET,name=spoils";
//    std::cout << "root: " << (*((*conf.begin()).get_routes().begin())).get_root()  << std::endl;
    std::pair<std::string, std::string>     p;
    p = (*conf.begin()).split_request(request);
//    std::cout << p.first << " " << p.second  << std::endl;
}