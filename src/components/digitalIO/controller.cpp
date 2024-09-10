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

DigitalIOController::DigitalIOController() {
  _dio_model = new DigitalIOModel();
  _dio_hardware = new DigitalIOHardware();
  // TODO: Should we create the model here, too?
  SetMaxDigitalPins(0);
}

DigitalIOController::~DigitalIOController() {
  delete _dio_model;
  delete _dio_hardware;
}

void DigitalIOController::SetMaxDigitalPins(uint8_t max_digital_pins) {
  _max_digital_pins = max_digital_pins;
}

bool DigitalIOController::IsStatusLEDPin(uint8_t pin_name) {
#ifdef STATUS_LED_PIN
  if (pin_name == STATUS_LED_PIN)
    return true;
#endif
  return false;
}

bool DigitalIOController::AddDigitalPin(pb_istream_t *stream) {
  // Attempt to decode the DigitalIOAdd message and parse it into the model
  if (!_dio_model->DecodeDigitalIOAdd(stream))
    return false; // Failed to decode the DigitalIOAdd message

  // Strip the D/A prefix off the pin name and convert to a uint8_t pin number
  int pin_name = atoi(_dio_model->GetDigitalIOAddMsg()->pin_name + 1);

  if (IsStatusLEDPin(pin_name))
    releaseStatusLED();

  // TODO: Only use one vector for all pins

  // Configure the pin based on the direction
  if (_dio_model->GetDigitalIOAddMsg()->gpio_direction ==
      wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_OUTPUT) {
    // Create a new DigitalOutputPin struct and add it to the vector
    DigitalIOPin new_pin;
    new_pin.pin_name = pin_name;
    new_pin.pin_value = _dio_model->GetDigitalIOAddMsg()->value;
    // Check if we have reached the maximum number of digital pins
    if (_digital_io_pins.size() >= _max_digital_pins) {
      WS_DEBUG_PRINTLN("ERROR: Can not add new digital pin, all pins have "
                       "already been allocated!");
      return false;
    }
    _digital_io_pins.push_back(new_pin);
    // Call the hardware
    _dio_hardware->SetPinMode(pin_name, true, false);
  } else if (
      _dio_model->GetDigitalIOAddMsg()->gpio_direction ==
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_INPUT ||
      _dio_model->GetDigitalIOAddMsg()->gpio_direction ==
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_INPUT_PULL_UP) {
    // TODO, this is not implemented yet!
    // TODO: Split this up for direction w/pull
  } else {
    return false; // Invalid pin direction specified
  }
  return true;
}

int DigitalIOController::GetDigitalOutputPinsIdx(uint8_t pin_name) {
  for (int i = 0; i < _digital_io_pins.size(); i++) {
    if (_digital_io_pins[i].pin_name == pin_name) {
      return i;
    }
  }
  return -1; // Pin not found
}

bool DigitalIOController::WriteDigitalPin(pb_istream_t *stream) {
  // Attempt to decode the DigitalIOWrite message
  if (!_dio_model->DecodeDigitalIOWrite(stream)) {
    WS_DEBUG_PRINTLN("ERROR: Unable to decode DigitalIOWrite message!");
    return false;
  }

  // Get the digital pin
  // TODO: Pull this all into a separate method
  int pin_idx = GetDigitalOutputPinsIdx(
      atoi(_dio_model->GetDigitalIOWriteMsg()->pin_name + 1));
  // Check if the pin was found and is a valid digital output pin
  if (pin_idx == -1) {
    WS_DEBUG_PRINTLN("ERROR: Unable to find the requested digital output pin!");
    return false;
  }

  // Ensure we got the correct value type
  if (!_dio_model->GetDigitalIOWriteMsg()->value.which_value ==
      wippersnapper_sensor_SensorEvent_bool_value_tag) {
    WS_DEBUG_PRINTLN("ERROR: DigitalIO controller got invalid value type!");
    return false;
  }

  // Is the pin already set to this value? If so, we don't need to write it
  // again
  if (_digital_io_pins[pin_idx].pin_value ==
      _dio_model->GetDigitalIOWriteMsg()->value.value.bool_value) {
    return true;
  }

  // Call hardware to write the value type
  _dio_hardware->WriteDigitalPin(
      _digital_io_pins[pin_idx].pin_name,
      _dio_model->GetDigitalIOWriteMsg()->value.value.bool_value);

  // Update the pin's value
  _digital_io_pins[pin_idx].pin_value =
      _dio_model->GetDigitalIOWriteMsg()->value.value.bool_value;

  return true;
}