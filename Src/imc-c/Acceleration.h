//***************************************************************************
// Copyright 2017 OceanScan - Marine Systems & Technology, Lda.             *
//***************************************************************************
// Licensed under the Apache License, Version 2.0 (the "License");          *
// you may not use this file except in compliance with the License.         *
// You may obtain a copy of the License at                                  *
//                                                                          *
// http://www.apache.org/licenses/LICENSE-2.0                               *
//                                                                          *
// Unless required by applicable law or agreed to in writing, software      *
// distributed under the License is distributed on an "AS IS" BASIS,        *
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
// See the License for the specific language governing permissions and      *
// limitations under the License.                                           *
//***************************************************************************
// Author: Ricardo Martins                                                  *
//***************************************************************************
// Automatically generated.                                                 *
//***************************************************************************
// IMC XML MD5: 522ff971d12877ebe15aff467ba253d4                            *
//***************************************************************************

#ifndef IMC_ACCELERATION_H_INCLUDED_
#define IMC_ACCELERATION_H_INCLUDED_

#include "Header.h"
#include "Serialization.h"

//! Acceleration.
typedef struct Acceleration
{
  Header m_header;
  //! Device Time.
  double time;
  //! X.
  double x;
  //! Y.
  double y;
  //! Z.
  double z;
} Acceleration;

static uint16_t
Acceleration_getIdStatic(void)
{
  return 257;
}

void
Acceleration_clear(Acceleration* accel)
{
  accel->time = 0;
  accel->x = 0;
  accel->y = 0;
  accel->z = 0;
}

Acceleration
Acceleration_new(void)
{
  Acceleration accel;
  accel.m_header.mgid = Acceleration_getIdStatic();
  Acceleration_clear(&accel);

  return accel;
}

size_t
Acceleration_getFixedSerializationSize(void)
{
  return 32;
}

uint8_t*
Acceleration_serialize(const Acceleration* accel, uint8_t* bfr__)
{
  uint8_t* ptr__ = bfr__;
  ptr__ += serializeHeader(&accel->m_header, Acceleration_getFixedSerializationSize(), ptr__);
  ptr__ += serialize(&accel->time, sizeof(accel->time),ptr__);
  ptr__ += serialize(&accel->x, sizeof(accel->x),ptr__);
  ptr__ += serialize(&accel->y, sizeof(accel->y),ptr__);
  ptr__ += serialize(&accel->z, sizeof(accel->z), ptr__);
  return ptr__;
}

uint16_t
Acceleration_getId(void)
{
  return Acceleration_getIdStatic();
}

const char*
Acceleration_getName(void)
{
  return "Acceleration";
}

#endif
