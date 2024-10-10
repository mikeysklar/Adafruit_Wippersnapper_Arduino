/*!
 * @file controller.h
 *
 * Controller for the DS18X20 API
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
#ifndef WS_DS18X20_CONTROLLER_H
#define WS_DS18X20_CONTROLLER_H
#include "Wippersnapper_V2.h"
#include "hardware.h"
#include "model.h"

class Wippersnapper_V2; ///< Forward declaration
class DS18X20Model;     ///< Forward declaration
class DS18X20Hardware;  ///< Forward declaration

/**************************************************************************/
/*!
    @brief  Routes messages between the ds18x20.proto API and the hardware.
*/
/**************************************************************************/
class DS18X20Controller {
public:
  DS18X20Controller();
  ~DS18X20Controller();
  // Routing
  bool Handle_Ds18x20Add(pb_istream_t *stream);
  // TODO: Implement Handle DS18x20 Remove message!
  // Polling
  void update();

private:
  DS18X20Model *_DS18X20_model; ///< ds18x20 model
  std::vector<DS18X20Hardware>
      _DS18X20_pins; ///< Vector containing multiple DS18X20Hardware objects
};
extern Wippersnapper_V2 WsV2; ///< Wippersnapper V2 instance
#endif                        // WS_DS18X20_CONTROLLER_H