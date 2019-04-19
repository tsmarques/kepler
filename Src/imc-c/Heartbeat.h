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

#ifndef IMC_HEARTBEAT_H_INCLUDED_
#define IMC_HEARTBEAT_H_INCLUDED_

//! Heartbeat.
typedef struct Heartbeat
{
  Header m_header;
} Heartbeat;

uint16_t
Heartbeat_getIdStatic(void)
{
  return 150;
}

Heartbeat
Heartbeat_new(void)
{
  Heartbeat heartbeat;
  heartbeat.m_header.mgid = Heartbeat_getIdStatic();

  return heartbeat;
}

void
Heartbeat_clear(void)
{ }


size_t
Heartbeat_getFixedSerializationSize(void)
{
  return 0;
}

uint8_t*
Heartbeat_serialize(const Heartbeat* heartbeat, uint8_t* bfr__)
{
  uint8_t* ptr__ = bfr__;
  ptr__ += serializeHeader(&heartbeat->m_header, 0, ptr__);
  return bfr__;
}

uint16_t
Heartbeat_getId(void)
{
  return Heartbeat_getIdStatic();
}

const char*
getName(void)
{
  return "Heartbeat";
}

#endif
