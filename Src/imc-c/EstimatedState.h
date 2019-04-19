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

#ifndef IMC_ESTIMATEDSTATE_H_INCLUDED_
#define IMC_ESTIMATEDSTATE_H_INCLUDED_

typedef struct EstimatedState
{
  Header m_header;
  //! Latitude (WGS-84).
  double lat;
  //! Longitude (WGS-84).
  double lon;
  //! Height (WGS-84).
  float height;
  //! Offset north.
  float x;
  //! Offset east.
  float y;
  //! Offset down.
  float z;
  //! Rotation over x axis.
  float phi;
  //! Rotation over y axis.
  float theta;
  //! Rotation over z axis.
  float psi;
  //! Body-Fixed xx Velocity.
  float u;
  //! Body-Fixed yy Velocity.
  float v;
  //! Body-Fixed zz Velocity.
  float w;
  //! Ground Velocity X (North).
  float vx;
  //! Ground Velocity Y (East).
  float vy;
  //! Ground Velocity Z (Down).
  float vz;
  //! Angular Velocity in x.
  float p;
  //! Angular Velocity in y.
  float q;
  //! Angular Velocity in z.
  float r;
  //! Depth.
  float depth;
  //! Altitude.
  float alt;
} EstimatedState;

uint16_t
EstimatedState_getIdStatic(void)
{
  return 350;
}


size_t
EstimatedState_getFixedSerializationSize()
{
  return 88;
}

void
EstimatedState_clear(EstimatedState* estate)
{
  estate->lat = 0;
  estate->lon = 0;
  estate->height = 0;
  estate->x = 0;
  estate->y = 0;
  estate->z = 0;
  estate->phi = 0;
  estate->theta = 0;
  estate->psi = 0;
  estate->u = 0;
  estate->v = 0;
  estate->w = 0;
  estate->vx = 0;
  estate->vy = 0;
  estate->vz = 0;
  estate->p = 0;
  estate->q = 0;
  estate->r = 0;
  estate->depth = 0;
  estate->alt = 0;
}

EstimatedState
EstimatedState_new(void)
{
  EstimatedState estate;
  estate.m_header.mgid = EstimatedState_getIdStatic();
  EstimatedState_clear(&estate);

  return estate;
}


uint8_t*
EstimatedState_serialize(const EstimatedState* estate, uint8_t* bfr__)
{
  uint8_t* ptr__ = bfr__;
  ptr__ += serializeHeader(&estate->m_header,
                           EstimatedState_getFixedSerializationSize(),
                           ptr__);

  ptr__ += serialize(&estate->lat, sizeof(estate->lat), ptr__);
  ptr__ += serialize(&estate->lon, sizeof(estate->lon), ptr__);
  ptr__ += serialize(&estate->height, sizeof(estate->height), ptr__);
  ptr__ += serialize(&estate->x, sizeof(estate->x), ptr__);
  ptr__ += serialize(&estate->y, sizeof(estate->y), ptr__);
  ptr__ += serialize(&estate->z, sizeof(estate->z), ptr__);
  ptr__ += serialize(&estate->phi, sizeof(estate->phi), ptr__);
  ptr__ += serialize(&estate->theta, sizeof(estate->theta), ptr__);
  ptr__ += serialize(&estate->psi, sizeof(estate->psi), ptr__);
  ptr__ += serialize(&estate->u, sizeof(estate->u), ptr__);
  ptr__ += serialize(&estate->v, sizeof(estate->v), ptr__);
  ptr__ += serialize(&estate->w, sizeof(estate->w), ptr__);
  ptr__ += serialize(&estate->vx, sizeof(estate->vx), ptr__);
  ptr__ += serialize(&estate->vy, sizeof(estate->vy), ptr__);
  ptr__ += serialize(&estate->vz, sizeof(estate->vz), ptr__);
  ptr__ += serialize(&estate->p, sizeof(estate->p), ptr__);
  ptr__ += serialize(&estate->q, sizeof(estate->q), ptr__);
  ptr__ += serialize(&estate->r, sizeof(estate->r), ptr__);
  ptr__ += serialize(&estate->depth, sizeof(estate->depth), ptr__);
  ptr__ += serialize(&estate->alt, sizeof(estate->alt), ptr__);
  return ptr__;
}


uint16_t
EstimatedState_getId(void)
{
  return EstimatedState_getIdStatic();
}

const char*
EstimatedState_getName(void)
{
  return "EstimatedState";
}

#endif
