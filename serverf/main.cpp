//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"
#include "config.hpp"

int main(int ac, char *av[], char *env[])
{
    (void)av;
    t_data              t;
    t_serv              serv;
    timeval             tv = init_timevals();
    config              config_class;
    std::list<server>   conf;


    if (ac == 2) {
		config_class = parse(av[1]);
		conf = config_class.servers;
		std::cout << "parse complete" << std::endl;
		std::cout << config_class << std::endl;
	}
    else if (ac == 1)
    {
        conf.push_back(default_config());
    }
    else {
		// fixme error_usage();
		std::cerr << "Config file expected!" << std::endl;
		return 1;
	}
//    std::cout << "root: " << (*((*conf.begin()).get_routes().begin())).get_root()  << std::endl;
    serv = init_serv();
    if (!(t.env = ft_doublecpy(env)))
        error_exit("malloc error");
//    tests(conf);
    loop(tv, serv, t, conf);
    return (0);
}
