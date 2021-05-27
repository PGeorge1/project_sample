#include "car.h"

Car::Car (const std::vector<QVariant> &data) : data (data)

{
}


const char *enum_to_string (car_fields field)
{
    switch (field)
    {
    case car_fields::number       : return "number";
    case car_fields::price        : return "price";
    case car_fields::brand        : return "brand";
    case car_fields::model        : return "model";
    case car_fields::year         : return "year";
    case car_fields::title_status : return "title_status";
    case car_fields::mileage      : return "mileage";
    case car_fields::color        : return "color";
    case car_fields::vin          : return "vin";
    case car_fields::lot          : return "lot";
    case car_fields::state        : return "state";
    case car_fields::country      : return "country";
    case car_fields::condition    : return "condition";
    case car_fields::COUNT : return "";
    }
}
