//
// Created by Pamula File on 6/15/21.
//
#include "header.h"

std::string get_current_date()
{
    const static std::string daysOfWeek[] = {
            "Sun",
            "Mon",
            "Tue",
            "Wed",
            "Thu",
            "Fri",
            "Sat"
    };
    const static std::string monthsOfYear[] = {
            "January",
            "Fubruary",
            "March",
            "April",
            "May",
            "June",
            "August"
            "September"
            "October"
            "November"
            "December"
    };
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    std::string date;
    std::string month;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    date = daysOfWeek[timeinfo->tm_wday] + ", ";
    strftime(buffer,sizeof(buffer),"%d",timeinfo);
    date += std::string(buffer) + " ";
    strftime(buffer,sizeof(buffer),"%m",timeinfo);
    date += monthsOfYear[atoi(buffer) - 1] + " ";
    strftime(buffer,sizeof(buffer),"%Y %H:%M:%S",timeinfo);
    date += std::string(buffer) + "\n";
    return date;
}
