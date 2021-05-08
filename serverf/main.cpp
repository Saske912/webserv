//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"

int main(int ac, char *av[])
{
    (void)av;
    (void)ac;
    t_data              t;
    t_serv              serv;
    timeval             tv = init_timevals();
    std::list<t_config> conf;

//    if (ac == 2)
//        parse_config(av[1]);
//    if (ac == 1)
//        *conf.begin() = default_config();
    serv = init_serv();
    loop(tv, serv, t);
    return (0);
}