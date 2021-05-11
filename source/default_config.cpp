//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"

server    default_config(void)
{
    char buf[150];

    std::string root = getcwd(buf, sizeof(buf));
    server c("10.21.31.71", 1024, std::map<int, std::string>(), \
            std::list<route>(1, route("/", root+"/content/", std::list<std::string>())));
//    std::cout << "root: " << (*(c.get_routes().begin())).get_root()  << std::endl;
    return c;
}