#include <iostream>
#include <ctime>
#include <sstream>
#include <cmath>

// void add_zero(int timer) {
//     if (timer >= 0 && timer < 10) {
//         Response.append("0");
//     }
// }

unsigned long   hex_to_dec(std::string hex) {
    unsigned long output = 0;
    for (int i = 0; i < hex.length(); ++i) {
        if (hex[i] >= 48 && hex[i] <= 57) {
            output += (hex[i] - 48)* std::pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 65 && hex[i] <= 70) {
            output += (hex[i] - 55)* std::pow(16, hex.length() - i - 1);
        }
        else if (hex[i] >= 97 && hex[i] <= 102) {
            output += (hex[i] - 87)* std::pow(16, hex.length() - i - 1);
        }
    }
    return output;
}

// int main() {
//     time_t now = time(0);
//     std::string days_of_week[8] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};
//     std::string months[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};
//     std::string result_day;
//     std::ostringstream convert;

//     tm *time = localtime(&now);
//     convert << time->tm_mday;
//     result_day.append(days_of_week[time->tm_wday]).append(" ").append(convert.str()).append(" ");
//     convert.str("");
//     convert.clear();
//     convert << (time->tm_year + 1900);
//     result_day.append(months[time->tm_mon]).append(" ").append(convert.str()).append(" ");
//     convert.str("");
//     convert.clear();
//     convert << time->tm_hour;
//     result_day.append(convert.str()).append(":");
//     convert.str("");
//     convert.clear();
//     convert << time->tm_min;
//     result_day.append(convert.str()).append(":");
//     convert.str("");
//     convert.clear();
//     convert << time->tm_sec;
//     result_day.append(convert.str()).append(" GMT");
//     convert.str("");
//     convert.clear();
//     std::cout <<  result_day << std::endl;

// }

int main() {
    std::string in = "98";

    std::cout << hex_to_dec(in) << std::endl;
}