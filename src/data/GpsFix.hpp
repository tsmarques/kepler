#ifndef KEPLER_GPSFIX_HPP
#define KEPLER_GPSFIX_HPP

#include "BasicData.hpp"

namespace kepler::data
{
  struct GpsFix : public BasicData
  {
    //! roll
    float m_lat;
    //! pitch
    float m_lon;
    //! yaw
    float m_height;

    GpsFix() :
        m_lat(0.0),
        m_lon(0.0),
        m_height(0.0)
    { }

    DataType
    getType() override
    {
      return DT_GPSFIX;
    }
  };
}

#endif
