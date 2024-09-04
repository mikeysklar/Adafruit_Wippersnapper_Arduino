/*!
 * @file controller.cpp
 *
 * Controller for the digitalio.proto API
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
#include "controller.h"

DigitalIOController::DigitalIOController() {}

DigitalIOController::~DigitalIOController() {}

void DigitalIOController::SetMaxDigitalPins(uint8_t max_digital_pins) {
  _max_digital_pins = max_digital_pins;
}

bool DigitalIOController::AddDigitalPin(pb_istream_t *stream) {
  // Attempt to decode the DigitalIOAdd message and parse it into the model
  if (!WsV2.digital_io_model->DecodeDigitalIOAdd(stream))
    return false; // Failed to decode the DigitalIOAdd message

  // Strip the D/A prefix off the pin name and convert to a uint8_t pin number
  int pin_name =
      atoi(WsV2.digital_io_model->GetDigitalIOAddMsg()->pin_name + 1);

  // Configure the pin based on the direction
  if (WsV2.digital_io_model->GetDigitalIOAddMsg()->gpio_direction ==
      wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_OUTPUT) {
    // Create a new DigitalOutputPin struct and add it to the vector
    DigitalOutputPin new_pin;
    new_pin.pin_name = pin_name;
    new_pin.pin_value = WsV2.digital_io_model->GetDigitalIOAddMsg()->value;
    // Check if we have reached the maximum number of digital pins
    if (_digital_output_pins.size() >= _max_digital_pins) {
      WS_DEBUG_PRINTLN("ERROR: Can not add new digital pin, all pins have "
                       "already been allocated!");
      return false;
    }
    _digital_output_pins.push_back(new_pin);
  } else if (
      WsV2.digital_io_model->GetDigitalIOAddMsg()->gpio_direction ==
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_INPUT ||
      WsV2.digital_io_model->GetDigitalIOAddMsg()->gpio_direction ==
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_INPUT_PULL_UP) {
    // TODO, this is not implemented yet!
  } else {
    return false; // Invalid pin direction specified
  }
  // Zero-out the DigitalIOAdd message struct.
  WsV2.digital_io_model->ClearDigitalIOAdd();

  return true;
}

DigitalOutputPin *DigitalIOController::GetDigitalOutputPin(uint8_t pin_name) {
  for (int i = 0; i < _digital_output_pins.size(); i++) {
    if (_digital_output_pins[i].pin_name == pin_name) {
      return &_digital_output_pins[i];
    }
  }
  return NULL;
}