//
// Created by Pamula File on 5/7/21.
//
#include "../header.h"

void    add_fd_to_set(int fd, t_data *t)
{
    FD_SET( fd, &t->read);
    FD_SET( fd, &t->write);
}
