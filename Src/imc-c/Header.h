#ifndef IMC_HEADER_H_INCLUDED_
#define IMC_HEADER_H_INCLUDED_

//! Header format.
typedef struct Header
{
  //! Synchronization Number.
  uint16_t sync;
  //! Message Identification Number.
  uint16_t mgid;
  //! Message size.
  uint16_t size;
  //! Time stamp. (This will be serialized as fp64_t)
  double timestamp;
  //! Source Address.
  uint16_t src;
  //! Source Entity.
  uint8_t src_ent;
  //! Destination Address.
  uint16_t dst;
  //! Destination Entity.
  uint8_t dst_ent;
} Header;

#endif
