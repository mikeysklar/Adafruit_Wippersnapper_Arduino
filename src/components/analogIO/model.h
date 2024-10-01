/*!
 * @file model.h
 *
 * Model interface for the analogio.proto message.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Copyright (c) Brent Rubell 2024 for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */
#ifndef WS_ANALOGIO_MODEL_H
#define WS_ANALOGIO_MODEL_H
#include "Wippersnapper_V2.h"

/**************************************************************************/
/*!
    @brief  Provides an interface for creating, encoding, and parsing
            messages from analogio.proto.
*/
/**************************************************************************/
class AnalogIOModel {
public:
  AnalogIOModel();
  ~AnalogIOModel();
  // AnalogIOAdd
  bool DecodeAnalogIOAdd(pb_istream_t *stream);
  wippersnapper_analogio_AnalogIOAdd *GetAnalogIOAddMsg();
  // AnalogIORemove
  bool DecodeAnalogIORemove(pb_istream_t *stream);
  wippersnapper_analogio_AnalogIORemove *GetAnalogIORemoveMsg();

private:
  wippersnapper_analogio_AnalogIOAdd _msg_AnalogioAdd;
  wippersnapper_analogio_AnalogIORemove _msg_AnalogioRemove;
};
#endif // WS_DIGITALIO_MODEL_H