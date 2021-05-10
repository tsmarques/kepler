#ifndef KEPLER_ATTITUDE_HPP
#define KEPLER_ATTITUDE_HPP

#include "BasicData.hpp"

namespace kepler::data
{
  struct Attitude : public BasicData
  {
    //! roll
    float m_phi;
    //! pitch
    float m_theta;
    //! yaw
    float m_psi;

    Attitude() :
        m_phi(0.0),
        m_theta(0.0),
        m_psi(0.0)
    { }

    DataType
    getType() override
    {
      return DT_ATTITUDE;
    }
  };
}

#endif
