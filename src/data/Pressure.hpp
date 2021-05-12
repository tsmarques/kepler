#ifndef KEPLER_PRESSURE_HPP
#define KEPLER_PRESSURE_HPP

#include "BasicData.hpp"

namespace kepler::data
{
  struct Pressure : public BasicData
  {
    float value;

    Pressure() :
        value(0.0)
    { }

    DataType
    getType() override
    {
      return DT_PRESSURE;
    }
  };
}

#endif
