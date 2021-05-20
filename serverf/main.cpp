//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"
#include "config.hpp"

void print_usage() {
    std::cerr << std::endl << "usage: webserv [file]" <<
        std::endl << std::endl << "arguments:" << std::endl <<
        "  file          Configuration file for webserver." <<
        " If no file was provided," << std::endl <<
        "                default config will be used." <<
        std::endl;
}

int main(int ac, char *av[], char *env[])
{
    t_data              t;
    t_serv              serv;
    timeval             tv = init_timevals();
    config              config_class;


    if (ac == 2) {
        config_class = parse(av[1]);
    }
    else if (ac == 1) {
        config_class = parse("cfg/ConfigExample.ws");
    }
    else {
        std::cerr << "Wrong number of arguments. Expected 0 or 1, got " <<
            (ac - 1) << "." << std::endl;
        print_usage();
        return 1;
    }
    serv = init_serv();
    if (!(t.env = ft_doublecpy(env)))
        error_exit("malloc error");
    loop(tv, serv, t, config_class.servers);
    return (0);
}
