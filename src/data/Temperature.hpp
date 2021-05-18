#ifndef KEPLER_TEMPERATURE_HPP
#define KEPLER_TEMPERATURE_HPP

#include "BasicData.hpp"

namespace kepler::data
{
  struct Temperature : public BasicData
  {
    float value;

    Temperature() :
        value(0.0)
    { }

    DataType
    getType() override
    {
      return DT_TEMPERATURE;
    }
  };
}

#endif
