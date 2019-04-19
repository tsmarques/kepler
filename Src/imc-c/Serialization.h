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

#ifndef IMC_SERIALIZATION_HPP_INCLUDED_
#define IMC_SERIALIZATION_HPP_INCLUDED_


#include <string.h>
#include "IMC.h"

inline size_t
serialize(const void* v, const uint16_t size, uint8_t *bfr)
{
  memcpy(bfr, v, size);
  return size;
}

inline uint16_t
serializeHeader(const Header* header, const uint16_t msg_ser_size, uint8_t* bfr)
{
  uint8_t* ptr = bfr;

  ptr += serialize((uint16_t*)DUNE_IMC_CONST_SYNC, 2, ptr);
  ptr += serialize(&header->mgid, sizeof(header->mgid), ptr);
  ptr += serialize(&msg_ser_size, sizeof(msg_ser_size), ptr);
  ptr += serialize(&header->timestamp, sizeof(header->timestamp), ptr);
  ptr += serialize(&header->src, sizeof(header->src), ptr);
  ptr += serialize(&header->src_ent, sizeof(header->src), ptr);
  ptr += serialize(&header->dst, sizeof(header->dst), ptr);
  ptr += serialize(&header->dst_ent, sizeof(header->dst), ptr);

  return DUNE_IMC_CONST_HEADER_SIZE;
}

#endif
