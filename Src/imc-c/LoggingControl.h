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

#ifndef IMC_LOGGINGCONTROL_H_INCLUDED_
#define IMC_LOGGINGCONTROL_H_INCLUDED_

#include "Header.h"
#include "Serialization.h"
#include "IMC.h"

//! Control Operation.
enum LoggingControl_ControlOperationEnum
{
  //! Request Start of Logging.
  COP_REQUEST_START = 0,
  //! Logging Started.
  COP_STARTED = 1,
  //! Request Logging Stop.
  COP_REQUEST_STOP = 2,
  //! Logging Stopped.
  COP_STOPPED = 3,
  //! Request Current Log Name.
  COP_REQUEST_CURRENT_NAME = 4,
  //! Current Log Name.
  COP_CURRENT_NAME = 5
};

//! Logging Control.
typedef struct LoggingControl
{
  Header m_header;
  //! Control Operation.
  uint8_t op;
  //! Log Label / Path.
  char name[IMC_MAX_LOG_NAME_SIZE];
} LoggingControl;

uint16_t
LoggingControl_getIdStatic(void)
{
  return 102;
}

void
LoggingControl_clear(LoggingControl* log_ctrl)
{
  log_ctrl->op = 0;
  log_ctrl->name[0] = 0;
}

LoggingControl
LoggingControl_new(void)
{
  LoggingControl log_c;
  log_c.m_header.mgid = LoggingControl_getIdStatic();
  LoggingControl_clear(&log_c);

  return log_c;
}

uint8_t*
LoggingControl_serialize(const LoggingControl* log_c, uint8_t* bfr__)
{
  uint8_t* ptr__ = bfr__;
  ptr__ += serialize(&log_c->op, sizeof(log_c->op), ptr__);
  ptr__ += serialize(&log_c->name, strlen(log_c->name), ptr__);
  return ptr__;
}

uint16_t
LoggingControl_getId(void)
{
  return LoggingControl_getIdStatic();
}

const char*
LoggingControl_getName(void)
{
  return "LoggingControl";
}

size_t
LoggingControl_getFixedSerializationSize(void)
{
  return 1;
}

#endif
