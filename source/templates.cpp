#include <cstdlib>
#include <cstring>
#include "header.h"

void    set_env( char **double_array, config &object )
{
	int		i;
	std::list<std::string>  env;

	if (!double_array)
		return ;
    i = -1;
	while (double_array[++i])
		env.push_back(double_array[i]);
	object.setEnv(env);
}