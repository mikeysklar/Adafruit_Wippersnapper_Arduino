/*!
 * @file controller.h
 *
 * Controller for the digitalio API
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
#ifndef WS_DIGITALIO_CONTROLLER_H
#define WS_DIGITALIO_CONTROLLER_H
#include "Wippersnapper_V2.h"
#include "hardware.h"
#include "model.h"

class Wippersnapper_V2;

struct DigitalIOPin {
  uint8_t pin_name;
  wippersnapper_digitalio_DigitalIODirection pin_direction;
  wippersnapper_digitalio_DigitalIOSampleMode sample_mode;
  bool pin_value;
  bool prv_pin_value;
  ulong pin_period;
  ulong prv_pin_time;
};

class DigitalIOModel;    // Forward declaration
class DigitalIOHardware; // Forward declaration

/**************************************************************************/
/*!
    @brief  Routes messages using the digitalio.proto API to the
            appropriate hardware and model classes, controls and tracks
            the state of the hardware's digital I/O pins.
*/
/**************************************************************************/
class DigitalIOController {
public:
  DigitalIOController();
  ~DigitalIOController();
  // Decoder-related Functions
  bool AddDigitalIOPin(pb_istream_t *stream);
  bool WriteDigitalIOPin(pb_istream_t *stream);
  // Encoder-related Functions
  void Update();
  bool EncodePublishPinEvent(uint8_t pin_name, bool pin_value);

  void
  CreateDigitalIOPin(uint8_t name,
                     wippersnapper_digitalio_DigitalIODirection direction,
                     wippersnapper_digitalio_DigitalIOSampleMode sample_mode,
                     bool value, long period);
  bool CheckEventPin(DigitalIOPin *pin);
  bool CheckTimerPin(DigitalIOPin *pin);
  bool IsPinTimerExpired(DigitalIOPin *pin, ulong cur_time);

  void PrintPinValue(DigitalIOPin *pin);
  void SetMaxDigitalPins(uint8_t max_digital_pins);
  int GetPinIdx(uint8_t pin_name);

private:
  std::vector<DigitalIOPin> _digital_io_pins;
  uint8_t _max_digital_pins;
  DigitalIOModel *_dio_model;
  DigitalIOHardware *_dio_hardware;
};
extern Wippersnapper_V2 WsV2;
#endif // WS_DIGITALIO_CONTROLLER_H