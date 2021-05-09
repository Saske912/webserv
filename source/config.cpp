//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"

t_config    default_config(void)
{
    char buf[150];

    std::string root = getcwd(buf, sizeof(buf));
    t_config c("127.0.0.1", 1024, std::map<int, std::string>(), \
            std::list<route>(1, route("/", root+"/content/", std::list<std::string>())));
//    std::cout << "root: " << (*(c.get_routes().begin())).get_root()  << std::endl;
    return c;
}