/*!
 * @file ws_sdcard.cpp
 *
 * Interface for Wippersnapper's SD card filesystem.
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
#include "ws_sdcard.h"

/**************************************************************************/
/*!
    @brief    Constructs an instance of the Wippersnapper SD card class.
*/
/**************************************************************************/
ws_sdcard::ws_sdcard() {
  _is_sd_card_inserted = false;
#ifndef SD_CS_PIN
  return;
#endif

  // Attempt to initialize the SD card
  if (_sd.begin(SD_CS_PIN)) {
    _is_sd_card_inserted = true;
  }
}

/**************************************************************************/
/*!
    @brief    Destructs an instance of the Wippersnapper SD card class.
*/
/**************************************************************************/
ws_sdcard::~ws_sdcard() {
  // TODO: Close any open files
  // Then, end the SD card (ends SPI transaction)
  _sd.end();
}

/**************************************************************************/
/*!
    @brief    Checks if an SD card is inserted and mounted.
    @returns  True if an SD card is inserted and mounted, False otherwise.
*/
/**************************************************************************/
bool ws_sdcard::IsSDCardInserted() { return _is_sd_card_inserted; }

bool ws_sdcard::parseConfigFile() {
  File32 file_config; // TODO: MAke this global?
#ifndef OFFLINE_MODE_DEBUG
  file_config = _sd.open("config.json", FILE_READ);
#endif

  JsonDocument doc;
  // TODO: Change max input length to fit an expected/max json size
  int max_input_len = 1024;

  // Attempt to de-serialize the JSON document
  DeserializationError error;
#ifdef OFFLINE_MODE_DEBUG
  _use_test_data = true; // TODO: This should be global
  if (!_use_test_data) {
    // Read the config file from the serial input buffer
    WS_DEBUG_PRINTLN("[SD] Reading JSON config file...");
    error = deserializeJson(doc, _serialInput.c_str(), max_input_len);
  } else {
    // Read the config file from the test JSON string
    WS_DEBUG_PRINTLN("[SD] Reading test JSON data...");
    error = deserializeJson(doc, json_test_data, max_input_len);
  }
#else
  // Read the config file from the SD card
  WS_DEBUG_PRINTLN("[SD] Reading config file...");
// TODO - implement this
// error = deserializeJson(doc, file_config, max_input_len);
#endif

  // If the JSON document failed to deserialize - halt the running device and
  // print the error because it is not possible to continue running in offline
  // mode without a valid config file
  if (error) {
    WS_DEBUG_PRINTLN("[SD] deserializeJson() failed, error code: " +
                     String(error.c_str()));
    return false;
  }

  // Parse the "components" array into a JsonObject
  JsonObject components = doc["components"][0];

  // TODO: This is a list so we'll need to refactor the following to loop thru,
  // parse and dispatch each component individually

  // Parse the PB API type
  const char *component_api_type =
      components["componentAPI"]; // ie: "analogio", "digitalio", etc.

  if (component_api_type == nullptr) {
    WS_DEBUG_PRINTLN("[SD] FATAL Parsing error - No component API type found in JSON string!");
    return false;
  } else {
    WS_DEBUG_PRINTLN("[SD] Component API type found: " + String(component_api_type));
  }

  // TODO- maybe a Switch case to handle the different component API types but
  // for now just a simple if-else is OK

  if (strcmp(component_api_type, "digitalio") == 0) {
    // Create a new digitalio add protobuf message
    wippersnapper_digitalio_DigitalIOAdd msg_DigitalIOAdd;
    msg_DigitalIOAdd = wippersnapper_digitalio_DigitalIOAdd_init_default;

    // Parse pinName
    strcpy(msg_DigitalIOAdd.pin_name, components["pinName"]);

    // Parse direction
    const char *direction = components["direction"];
    if (strcmp(direction, "INPUT") == 0) {
      msg_DigitalIOAdd.gpio_direction =
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_INPUT;
    } else if (strcmp(direction, "INPUT-PULLUP") == 0) {
      msg_DigitalIOAdd.gpio_direction =
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_INPUT_PULL_UP;
    } else if (strcmp(direction, "OUTPUT") == 0) {
      msg_DigitalIOAdd.gpio_direction =
          wippersnapper_digitalio_DigitalIODirection_DIGITAL_IO_DIRECTION_OUTPUT;
    } else { // Unknown direction, bail out
      WS_DEBUG_PRINTLN("Unknown digital pin direction found: " +
                       String(direction));
      return false;
    }

    // Determine the sample mode
    bool is_timer_sample_mode = false;
    const char *sample_mode = components["sampleMode"];
    if (strcmp(sample_mode, "TIMER") == 0)
      is_timer_sample_mode = true;

    if (is_timer_sample_mode) {
      // If we're sampling periodically, parse the period
      msg_DigitalIOAdd.period = components["timer"];
      // and set the sample mode
      msg_DigitalIOAdd.sample_mode =
          wippersnapper_digitalio_DigitalIOSampleMode_DIGITAL_IO_SAMPLE_MODE_TIMER;
    } else {
      // set the sample mode for event
      msg_DigitalIOAdd.sample_mode =
          wippersnapper_digitalio_DigitalIOSampleMode_DIGITAL_IO_SAMPLE_MODE_EVENT;
    }

    // Print out the contents of the DigitalIOADD message
    WS_DEBUG_PRINTLN("[SD] DigitalIOAdd message:");
    WS_DEBUG_PRINTLN("Pin Name: " + String(msg_DigitalIOAdd.pin_name));
    WS_DEBUG_PRINTLN("Direction: " + String(direction));
    WS_DEBUG_PRINTLN("Sample Mode: " + String(sample_mode));
    WS_DEBUG_PRINTLN("Period: " + String(msg_DigitalIOAdd.period));
    return true;
  } else if (strcmp(component_api_type, "analogio") == 0) {
    // TODO - dispatch to create analogio component protobuf message
    // Parse the AnalogIOAdd message
    wippersnapper_analogio_AnalogIOAdd msg_AnalogIOAdd =
        wippersnapper_analogio_AnalogIOAdd_init_default;
    // Fill in the AnalogIOAdd message
    strcpy(msg_AnalogIOAdd.pin_name, components["pinName"]);
    msg_AnalogIOAdd.period = components["period"];
    // Parse the analog pin's read mode
    if (strcmp(components["analogReadMode"], "PIN_VALUE") == 0) {
      msg_AnalogIOAdd.read_mode =
          wippersnapper_sensor_SensorType_SENSOR_TYPE_RAW;
    } else if (strcmp(components["analogReadMode"], "VOLTAGE") == 0) {
      msg_AnalogIOAdd.read_mode =
          wippersnapper_sensor_SensorType_SENSOR_TYPE_VOLTAGE;
    } else {
      // Unknown analog read mode, bail out
      WS_DEBUG_PRINTLN("[SD] Unknown analog read mode found: " +
                       String(components["analogReadMode"]));
      return false;
    }

    // Print out the contents of the AnalogIOAdd message
    WS_DEBUG_PRINTLN("[SD] AnalogIOAdd message:");
    WS_DEBUG_PRINTLN("Pin Name: " + String(msg_AnalogIOAdd.pin_name));
    WS_DEBUG_PRINTLN("Period: " + String(msg_AnalogIOAdd.period));
    WS_DEBUG_PRINTLN("Read Mode: " + String(msg_AnalogIOAdd.read_mode));
  } else {
    // Unknown component API type
    WS_DEBUG_PRINTLN("[SD] Unknown component API type found: " +
                     String(component_api_type));
    return false;
  }

  // TODO: Note that the TOP-LEVEL decoder is actually looking
  // for a SIGNAL message with one of these SUBMESSAGEd
  // so we'll need to encode this into a signalproto form before sending
  // it over

  return true;
}

// Returns true if input points to a valid JSON string
bool ws_sdcard::validateJson(const char *input) {
  JsonDocument doc, filter;

  DeserializationError error =
      deserializeJson(doc, input, DeserializationOption::Filter(filter));
  WS_DEBUG_PRINTLN("Error: " + String(error.c_str()));
  return error == DeserializationError::Ok;
  // return deserializeJson(doc, input, DeserializationOption::Filter(filter))
  // ==
  //         DeserializationError::Ok;
}

// Waits for incoming config file and parses it
// TODO: Split out parsing into parseConfigFile() and just read here
bool ws_sdcard::waitForSerialConfig() {

  // We provide three ways to use this function:
  // 1. Use a SD card with a JSON config file
  // 2. Provide a JSON string via the hardware's serial input
  // 3. Use a test JSON string - for debugging purposes ONLY
  json_test_data =
      "{\"components\":[{\"componentAPI\":\"analogio\",\"name\":\"Analog "
      "Pin\",\"pinName\":\"A18\",\"type\":\"analog_pin\",\"mode\":\"ANALOG\","
      "\"direction\":\"INPUT\",\"sampleMode\":\"TIMER\",\"analogReadMode\":"
      "\"PIN_VALUE\",\"period\":30,\"isPin\":true}]}\\n\r\n";
  _use_test_data = true;

  _serialInput = ""; // Clear the serial input buffer
  if (!_use_test_data) {
    WS_DEBUG_PRINTLN("[SD] Waiting for incoming JSON string...");
    while (true) {
      // Check if there is data available to read
      if (Serial.available() > 0) {
        // Read and append to _serialInput
        char c = Serial.read();
        _serialInput += c;
        // Check for EoL or end of JSON string
        // Read the TODO/Note below!
        // NOTE: This is checking for a \n delimeter from the serial
        // and that wont be present in non-serial application
        // Parse JSON normally if not using serial and inspect this condition!
        if (c == '\n') {
          break;
        }
      }
    }
  }

  // Print out the received JSON string
  WS_DEBUG_PRINT("[SD][Debug] JSON string received: ");
  if (_use_test_data) {
    WS_DEBUG_PRINTLN("[from json test data]");
    WS_DEBUG_PRINTLN(json_test_data);
  } else {
    WS_DEBUG_PRINTLN(_serialInput);
  }

  // Attempt to validate the string as JSON
  if (!_use_test_data) {
    if (!validateJson(_serialInput.c_str())) {
      WS_DEBUG_PRINTLN("[SD] Invalid JSON string received!");
      return false;
    }
  } else {
    if (!validateJson(json_test_data)) {
      WS_DEBUG_PRINTLN("[SD] Invalid JSON string received!");
      return false;
    }
  }

  WS_DEBUG_PRINTLN("[SD] Valid JSON string received!");
  return true;
}