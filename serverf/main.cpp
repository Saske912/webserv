//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"
#include "config.hpp"

int main(int ac, char *av[])
{
    (void)av;
    t_data              t;
    t_serv              serv;
    timeval             tv = init_timevals();
    std::list<server> conf;
    config config_class;

//    if (ac == 1)
	conf.push_back(default_config());
    if (ac == 2) {
		config_class = parse(av[1]);
		std::cout << "parse complete" << std::endl;
		std::cout << config_class << std::endl;
	}
    else {
		// fixme error_usage();
		std::cerr << "Config file expected!" << std::endl;
		return 1;
	}
//    std::cout << "root: " << (*((*conf.begin()).get_routes().begin())).get_root()  << std::endl;
    serv = init_serv();
    tests(conf);
    loop(tv, serv, t, conf);
    return (0);
}
