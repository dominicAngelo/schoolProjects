#ifndef DATE_H
#define DATE_H

#include <string>
#include <ctime>

class Date {
    int day;
    int month;
    int year;

    public:
        Date(int d, int m, int y);
        std::string toString() const;

        static Date getCurrentDate();
        
        bool operator>(const Date& other) const;
        bool operator<(const Date& other) const;
        bool operator==(const Date& other) const;
};

#endif