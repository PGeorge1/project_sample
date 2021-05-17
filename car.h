#ifndef CAR_H
#define CAR_H

#include <QDate>
#include <iostream>

enum class car_fields
{
    date,
    model,
    registration_number,
    horsepower,
    state,
    COUNT
};

class Car
{
public:
    std::vector<QVariant> data;
    Car (const std::vector<QVariant> &data);
};


#endif // CAR_H
