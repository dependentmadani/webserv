#include <iostream>
#include <ctime>
#include <sstream>

void add_zero(int timer) {
    if (timer >= 0 && timer < 10) {
        Response.append("0");
    }
}

int main() {
    time_t now = time(0);
    std::string days_of_week[8] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
    std::string months[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
    std::string result_day;
    std::ostringstream convert;

    tm *time = localtime(&now);
    convert << time->tm_mday;
    result_day.append(days_of_week[time->tm_wday]).append(" ").append(convert.str()).append(" ");
    convert.str("");
    convert.clear();
    convert << (time->tm_year + 1900);
    result_day.append(months[time->tm_mon]).append(" ").append(convert.str()).append(" ");
    convert.str("");
    convert.clear();
    convert << time->tm_hour;
    result_day.append(convert.str()).append(":");
    convert.str("");
    convert.clear();
    convert << time->tm_min;
    result_day.append(convert.str()).append(":");
    convert.str("");
    convert.clear();
    convert << time->tm_sec;
    result_day.append(convert.str()).append(" GMT");
    convert.str("");
    convert.clear();
    std::cout <<  result_day << std::endl;

}