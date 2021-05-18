#ifndef KEPLER_DATA_HPP
#define KEPLER_DATA_HPP

namespace kepler
{
  constexpr uint16_t c_sync = 0x1202;

  enum DataType
  {
    DT_ATTITUDE = 0,
    DT_GPSFIX,
    DT_PRESSURE,
    DT_TEMPERATURE,
  };

  class BasicData
  {
  public:
    virtual DataType getType() = 0;
  };
}

#endif
