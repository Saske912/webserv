//
// Created by Pamula File on 5/13/21.
//

#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

template<typename T, typename R>
bool    in_data(T const & data, R src)
{
    T::iterator     it = data.begin();
    while (it != data.end())
    {
        if (*it == src)
            return true;
        it++;
    }
    return false;
}

#endif //TEMPLATE_HPP
