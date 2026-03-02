#include "date.h"

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {
    // set the values and do nothing
}

std::string Date::toString() const {
    return std::to_string(month) + "/" + std::to_string(day) + "/" + std::to_string(year);
}

bool Date::operator>(const Date& other) const {

    if (year > other.year) {
        return true;
    } else if (year == other.year) {
        if (month > other.month) {
            return true;
        } else if (month == other.month) {
            return day > other.day;
        }
    }
    return false;
}

bool Date::operator<(const Date& other) const {
    if (this > &other) {
        return false;
    } else if (this == &other) {
        return false;
    } else {
        return true;
    }
}

bool Date::operator==(const Date& other) const {
    if (year == other.year && month == other.month && day == other.day) {
        return true;
    } else {
        return false;
    }
}