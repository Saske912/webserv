//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"

server    default_config(void)
{
    char buf[150];

    std::string root = getcwd(buf, sizeof(buf));
<<<<<<< HEAD
    server c("10.21.22.31", 1024, std::map<int, std::string>(), \
=======
    server c("10.21.22.33", 1024, std::map<int, std::string>(), \
>>>>>>> 06c5dffcea5b447e10e16da98c13693c165dace0
            std::list<route>(1, route("/", root+"/content/", std::list<std::string>())));
//    std::cout << "root: " << (*(c.get_routes().begin())).get_root()  << std::endl;
    return c;
}
<<<<<<< HEAD
=======

>>>>>>> 06c5dffcea5b447e10e16da98c13693c165dace0
