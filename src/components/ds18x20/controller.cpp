/*!
 * @file controller.cpp
 *
 * Controller for the ds18x20.proto API
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

/***********************************************************************/
/*!
    @brief  DS18X20Controller constructor
*/
/***********************************************************************/
DS18X20Controller::DS18X20Controller() { _DS18X20_model = new DS18X20Model(); }

/***********************************************************************/
/*!
    @brief  DS18X20Controller destructor
*/
/***********************************************************************/
DS18X20Controller::~DS18X20Controller() { delete _DS18X20_model; }

/***********************************************************************/
/*!
    @brief  Handles a Ds18x20Add message from the broker. Attempts to
            initialize a  OneWire bus on the requested pin, attempts to
            initialize a DSTherm driver on the OneWire bus, adds the
            OneWire bus to the controller, and publishes a Ds18x20Added
            message to the broker indicating the result of this function.
    @param  stream
            The nanopb input stream.
    @return True if the sensor was successfully initialized,
            added to the controller, and a response was succesfully
            published to the broker. False otherwise.
*/
/***********************************************************************/
bool DS18X20Controller::Handle_Ds18x20Add(pb_istream_t *stream) {
  unsigned long total_start_time = millis();

  // Attempt to decode the incoming message into a Ds18x20Add message
  unsigned long decode_start_time = millis();
  if (!_DS18X20_model->DecodeDS18x20Add(stream)) {
    WS_DEBUG_PRINTLN("ERROR: Unable to decode Ds18x20Add message");
    return false;
  }
  unsigned long decode_end_time = millis();
  WS_DEBUG_PRINT("Decode time: ");
  WS_DEBUG_PRINTLN(decode_end_time - decode_start_time);

  // If we receive no sensor types to configure, bail out
  if (_DS18X20_model->GetDS18x20AddMsg()->sensor_types_count == 0) {
    WS_DEBUG_PRINTLN("ERROR: No ds18x sensor types provided!");
    return false;
  }

  // Extract the OneWire pin from the message
  uint8_t pin_name = atoi(_DS18X20_model->GetDS18x20AddMsg()->onewire_pin + 1);

  // Initialize the DS18X20Hardware object
  unsigned long init_start_time = millis();
  auto new_dsx_driver = std::make_unique<DS18X20Hardware>(pin_name);
  // Attempt to get the sensor's ID on the OneWire bus to show it's been init'd
  bool is_initialized = new_dsx_driver->GetSensor();
  unsigned long init_end_time = millis();
  WS_DEBUG_PRINT("Initialization time: ");
  WS_DEBUG_PRINTLN(init_end_time - init_start_time);

  if (is_initialized) {
    WS_DEBUG_PRINTLN("Sensor found on OneWire bus and initialized");

    // Set the sensor's resolution
    new_dsx_driver->SetResolution(
        _DS18X20_model->GetDS18x20AddMsg()->sensor_resolution);

    // Set the sensor's period
    new_dsx_driver->SetPeriod(_DS18X20_model->GetDS18x20AddMsg()->period);

    // Configure the types of sensor reads to perform
    unsigned long config_types_start_time = millis();
    for (int i = 0; i < _DS18X20_model->GetDS18x20AddMsg()->sensor_types_count;
         i++) {
      if (_DS18X20_model->GetDS18x20AddMsg()->sensor_types[i] ==
          wippersnapper_sensor_SensorType_SENSOR_TYPE_OBJECT_TEMPERATURE) {
        new_dsx_driver->is_read_temp_c = true;
      } else if (
          _DS18X20_model->GetDS18x20AddMsg()->sensor_types[i] ==
          wippersnapper_sensor_SensorType_SENSOR_TYPE_OBJECT_TEMPERATURE_FAHRENHEIT) {
        new_dsx_driver->is_read_temp_f = true;
      }
    }

    // Add the DS18X20Hardware object to the vector of hardware objects
    _DS18X20_pins.push_back(std::move(new_dsx_driver));
  } else {
    WS_DEBUG_PRINTLN(
        "ERROR: Unable to get ds18x sensor ID, ds18x sensor not initialized");
  }

  // Encode and publish a Ds18x20Added message back to the broker
  unsigned long encode_start_time = millis();
  if (!_DS18X20_model->EncodeDS18x20Added(
          _DS18X20_model->GetDS18x20AddMsg()->onewire_pin, is_initialized)) {
    WS_DEBUG_PRINTLN("ERROR: Unable to encode Ds18x20Added message");
    return false;
  }
  unsigned long encode_end_time = millis();
  WS_DEBUG_PRINT("Encode message time: ");
  WS_DEBUG_PRINTLN(encode_end_time - encode_start_time);

  // Publish the AnalogIO message to the broker
  unsigned long publish_start_time = millis();
  if (!WsV2.PublishSignal(wippersnapper_signal_DeviceToBroker_ds18x20_added_tag,
                          _DS18X20_model->GetDS18x20AddedMsg())) {
    WS_DEBUG_PRINTLN("ERROR: Unable to publish Ds18x20Added message");
    return false;
  }
  unsigned long publish_end_time = millis();
  WS_DEBUG_PRINT("Publish message time: ");
  WS_DEBUG_PRINTLN(publish_end_time - publish_start_time);

  unsigned long total_end_time = millis();
  WS_DEBUG_PRINT("Total function execution time: ");
  WS_DEBUG_PRINTLN(total_end_time - total_start_time);

  return true;
}


/***********************************************************************/
/*!
    @brief  Handles a Ds18x20Remove message from the broker. Attempts to
            remove a DS18X20Hardware object from the controller and
            release the OneWire pin for other uses.
    @param  stream
            The nanopb input stream.
    @return True if the sensor was successfully removed from the
            controller, False otherwise.
*/
/***********************************************************************/
bool DS18X20Controller::Handle_Ds18x20Remove(pb_istream_t *stream) {
  WS_DEBUG_PRINT("Removing DS18X20 sensor...");
  // Attempt to decode the stream
  if (!_DS18X20_model->DecodeDS18x20Remove(stream)) {
    WS_DEBUG_PRINTLN("ERROR: Unable to decode Ds18x20Remove message");
    return false;
  }
  // Create a temp. instance of the Ds18x20Remove message
  wippersnapper_ds18x20_Ds18x20Remove *msg_remove =
      _DS18X20_model->GetDS18x20RemoveMsg();
  uint8_t pin_name = atoi(msg_remove->onewire_pin + 1);

  // Find the driver/bus in the vector and remove it
  for (size_t i = 0; i < _DS18X20_pins.size(); ++i) {
    if (_DS18X20_pins[i]->GetOneWirePin() == pin_name) {
      _DS18X20_pins.erase(_DS18X20_pins.begin() + i);
      return true;
    }
  }
  WS_DEBUG_PRINT("Removed!");
  return true;
}

/***********************************************************************/
/*!
    @brief  Update/polling loop for the DS18X20 controller.
*/
/***********************************************************************/
void DS18X20Controller::update() {
  unsigned long total_start_time = millis();

  // Bail out if there are no OneWire pins to poll
  if (_DS18X20_pins.empty())
    return;

  // Iterate through the vector
  for (uint8_t i = 0; i < _DS18X20_pins.size(); i++) {
    unsigned long sensor_start_time = millis();

    // Create a reference to the DS18X20Hardware object
    DS18X20Hardware &temp_dsx_driver = *(_DS18X20_pins[i]);

    // Check if the driver's timer has not expired yet
    if (!temp_dsx_driver.IsTimerExpired()) {
      continue;
    }

    // Create a new sensor_event
    _DS18X20_model->InitDS18x20EventMsg();

    // Are we reading the temperature in Celsius, Fahrenheit, or both?
    if (temp_dsx_driver.is_read_temp_c) {
      unsigned long temp_c_start_time = millis();

      WS_DEBUG_PRINTLN("Reading temperature in Celsius"); // TODO: Debug remove
      // Attempt to read the temperature in Celsius
      if (!temp_dsx_driver.ReadTemperatureC()) {
        WS_DEBUG_PRINTLN("ERROR: Unable to read temperature in Celsius");
        continue;
      }
      float temp_c = temp_dsx_driver.GetTemperatureC();
      _DS18X20_model->addSensorEvent(
          wippersnapper_sensor_SensorType_SENSOR_TYPE_OBJECT_TEMPERATURE,
          temp_c);

      unsigned long temp_c_end_time = millis();
      WS_DEBUG_PRINT("Read temperature Celsius time: ");
      WS_DEBUG_PRINTLN(temp_c_end_time - temp_c_start_time);
    }
    if (temp_dsx_driver.is_read_temp_f) {
      unsigned long temp_f_start_time = millis();

      WS_DEBUG_PRINTLN("Reading temperature in Fahrenheit"); // TODO: Debug remove
      // Attempt to read the temperature in Fahrenheit
      if (!temp_dsx_driver.ReadTemperatureF()) {
        WS_DEBUG_PRINTLN("ERROR: Unable to read temperature in Fahrenheit");
        continue;
      }
      float temp_f = temp_dsx_driver.GetTemperatureF();
      _DS18X20_model->addSensorEvent(
          wippersnapper_sensor_SensorType_SENSOR_TYPE_OBJECT_TEMPERATURE_FAHRENHEIT,
          temp_f);

      unsigned long temp_f_end_time = millis();
      WS_DEBUG_PRINT("Read temperature Fahrenheit time: ");
      WS_DEBUG_PRINTLN(temp_f_end_time - temp_f_start_time);
    }

    // Get and print out the SensorEvent message's contents
    wippersnapper_ds18x20_Ds18x20Event event_msg =
        *_DS18X20_model->GetDS18x20EventMsg();
    pb_size_t event_count = event_msg.sensor_events_count;

    WS_DEBUG_PRINT("Sensor OneWire bus pin: ");
    WS_DEBUG_PRINT(temp_dsx_driver.GetOneWirePin());
    WS_DEBUG_PRINT(" got ");
    WS_DEBUG_PRINT(event_count);
    WS_DEBUG_PRINTLN(" sensor events");
    for (int i = 0; i < event_count; i++) {
      WS_DEBUG_PRINT("Sensor value: ");
      WS_DEBUG_PRINTLN(event_msg.sensor_events[i].value.float_value);
    }

    // Encode the Ds18x20Event message
    unsigned long encode_start_time = millis();
    if (!_DS18X20_model->EncodeDs18x20Event()) {
      WS_DEBUG_PRINTLN("ERROR: Failed to encode Ds18x20Event message");
      continue;
    }
    unsigned long encode_end_time = millis();
    WS_DEBUG_PRINT("Encode event message time: ");
    WS_DEBUG_PRINTLN(encode_end_time - encode_start_time);

    // Publish the Ds18x20Event message to the broker
    unsigned long publish_start_time = millis();
    WS_DEBUG_PRINT("Publishing Ds18x20Event message to broker...");
    if (!WsV2.PublishSignal(
            wippersnapper_signal_DeviceToBroker_ds18x20_event_tag,
            _DS18X20_model->GetDS18x20EventMsg())) {
      WS_DEBUG_PRINTLN("ERROR: Failed to publish Ds18x20Event message");
      continue;
    }
    WS_DEBUG_PRINTLN("Published!");
    unsigned long publish_end_time = millis();
    WS_DEBUG_PRINT("Publish event message time: ");
    WS_DEBUG_PRINTLN(publish_end_time - publish_start_time);

    unsigned long sensor_end_time = millis();
    WS_DEBUG_PRINT("Total sensor processing time: ");
    WS_DEBUG_PRINTLN(sensor_end_time - sensor_start_time);
  }

  unsigned long total_end_time = millis();
  WS_DEBUG_PRINT("Total update() execution time: ");
  WS_DEBUG_PRINTLN(total_end_time - total_start_time);
}
