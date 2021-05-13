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
	}
    else if (ac == 1)
    {
        char str[] = "cfg/ConfigExample.ws";
        config_class = parse(str);
        conf = config_class.servers;
    }
    else {
		// fixme error_usage();
		std::cerr << "Config file expected!" << std::endl;
		return 1;
	}
    serv = init_serv();
    if (!(t.env = ft_doublecpy(env)))
        error_exit("malloc error");
    loop(tv, serv, t, conf);
    return (0);
}
