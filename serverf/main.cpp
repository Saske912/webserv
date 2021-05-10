//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"

int main(int ac, char *av[], char *env[])
{
    (void)av;
//    (void)ac;
    t_data              t;
    t_serv              serv;
    timeval             tv = init_timevals();
    std::list<server>   conf;

//    if (ac == 2)
//       conf =  parse_config(av[1]);
    if (ac == 1)
        conf.push_back(default_config());
    serv = init_serv();
    tests(conf);
    loop(tv, serv, t, conf);
    return (0);
}