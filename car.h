#ifndef CAR_H
#define CAR_H

#include <QDate>
#include <iostream>

enum class car_fields
{
    number,
    price,
    brand,
    model,
    year,
    title_status,
    mileage,
    color,
    vin,
    lot,
    state,
    country,
    condition,
    COUNT
};

const char *enum_to_string (car_fields field);

class Car
{
public:
    std::vector<QVariant> data;
    Car (const std::vector<QVariant> &data);
};


#endif // CAR_H
