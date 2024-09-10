/*!
 * @file model.h
 *
 * Model for the digitalio.proto message.
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
#ifndef WS_DIGITALIO_HARDWARE_H
#define WS_DIGITALIO_HARDWARE_H
#include "Wippersnapper_V2.h"

class DigitalIOHardware {
public:
  DigitalIOHardware();
  ~DigitalIOHardware();
  bool ConfigurePin(uint8_t name,
                    wippersnapper_digitalio_DigitalIODirection direction);
  void WriteDigitalPin(uint8_t pin_name, bool pin_value);
  void deinit(uint8_t pin_name);

private:
};
#endif // WS_DIGITALIO_HARDWARE_H