/*!
 * @file Wippersnapper.h
 *
 * This is the documentation for Adafruit's Wippersnapper firmware for the
 * Arduino platform. It is designed specifically to work with
 * Adafruit IO Wippersnapper IoT platform.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Copyright (c) Brent Rubell 2020-2024 for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef WIPPERSNAPPER_V2_H
#define WIPPERSNAPPER_V2_H

// Cpp STD
#include <vector>

// Nanopb dependencies
#include <nanopb/pb_common.h>
#include <nanopb/pb_decode.h>
#include <nanopb/pb_encode.h>
#include <pb.h>

#include <wippersnapper/description/v1/description.pb.h> // description.proto
#include <wippersnapper/signal/v1/signal.pb.h>           // signal.proto

// External libraries
#include "Adafruit_MQTT.h"      // MQTT Client
#include "Adafruit_SleepyDog.h" // Watchdog
#include "Arduino.h"            // Wiring
#include <SPI.h>                // SPI

// Wippersnapper API Helpers
#include "Wippersnapper_Boards.h"
#include "components/statusLED/Wippersnapper_StatusLED.h"
#include "helpers/ws_helper_status.h"
#include "provisioning/ConfigJson.h"

// Wippersnapper pb helpers
#include <nanopb/ws_pb_helpers.h>

// Wippersnapper components
#include "components/analogIO/Wippersnapper_AnalogIO.h"
#include "components/digitalIO/Wippersnapper_DigitalGPIO.h"
#include "components/i2c/WipperSnapper_I2C.h"

// Includes for ESP32-only
#ifdef ARDUINO_ARCH_ESP32
#include "components/ledc/ws_ledc.h"
#include <Esp.h>
#endif

// Display
#ifdef USE_DISPLAY
#include "display/ws_display_driver.h"
#include "display/ws_display_ui_helper.h"
#endif

#include "components/ds18x20/ws_ds18x20.h"
#include "components/pixels/ws_pixels.h"
#include "components/pwm/ws_pwm.h"
#include "components/servo/ws_servo.h"
#include "components/uart/ws_uart.h"

#if defined(USE_TINYUSB)
#include "provisioning/tinyusb/Wippersnapper_FS_V2.h"
#endif

#if defined(USE_LITTLEFS)
#include "provisioning/littlefs/WipperSnapper_LittleFS.h"
#endif

#define WS_VERSION                                                             \
  "1.0.0-rc.1" ///< WipperSnapper app. version (semver-formatted)

#define WS_WDT_TIMEOUT 60000       ///< WDT timeout
#define WS_MAX_ALT_WIFI_NETWORKS 3 ///< Maximum number of alternative networks
/* MQTT Configuration */
#define WS_KEEPALIVE_INTERVAL_MS                                               \
  5000 ///< Session keepalive interval time, in milliseconds

#define WS_MQTT_MAX_PAYLOAD_SIZE                                               \
  512 ///< MAXIMUM expected payload size, in bytes

class Wippersnapper_DigitalGPIO;
class Wippersnapper_AnalogIO;
class Wippersnapper_FS_V2;
class WipperSnapper_LittleFS;
#ifdef USE_DISPLAY
class ws_display_driver;
class ws_display_ui_helper;
#endif
#ifdef ARDUINO_ARCH_ESP32
class ws_ledc;
#endif
class WipperSnapper_Component_I2C;
class ws_servo;
class ws_pwm;
class ws_ds18x20;
class ws_pixels;
class ws_uart;

/**************************************************************************/
/*!
    @brief  Class that provides storage and functions for the Adafruit IO
            Wippersnapper interface.
*/
/**************************************************************************/
class Wippersnapper_V2 {
public:
  Wippersnapper_V2();
  virtual ~Wippersnapper_V2();

  void provisionV2();

  bool
      lockStatusNeoPixelV2; ///< True if status LED is using the status neopixel
  bool lockStatusDotStarV2; ///< True if status LED is using the status dotstar
  bool lockStatusLEDV2;     ///< True if status LED is using the built-in LED
  float status_pixel_brightnessV2 =
      STATUS_PIXEL_BRIGHTNESS_DEFAULT; ///< Global status pixel's brightness
                                       ///< (from 0.0 to 1.0)

  virtual void set_user_keyV2();
  virtual void set_ssid_passV2(const char *ssid, const char *ssidPassword);
  virtual void set_ssid_passV2();
  virtual bool check_valid_ssidV2();

  virtual void _connectV2();
  virtual void _disconnectV2();
  void connectV2();
  void disconnectV2();

  virtual void getMacAddrV2();
  virtual int32_t getRSSIV2();
  virtual void setupMQTTClientV2(const char *clientID);

  virtual ws_status_t networkStatusV2();
  ws_board_status_t getBoardStatusV2();

  bool generateDeviceUIDV2();
  bool generateWSTopicsV2();
  // bool generateWSErrorTopicsV2();

  // Registration API
  bool registerBoardV2();
  bool encodePubRegistrationReqV2();
  void decodeRegistrationRespV2(char *data, uint16_t len);
  void pollRegistrationRespV2();
  // Configuration API
  void publishPinConfigCompleteV2();

  // run() loop
  ws_status_t runV2();
  void processPacketsV2();
  void publishV2(const char *topic, uint8_t *payload, uint16_t bLen,
                 uint8_t qos = 0);

  // Networking helpers
  void pingBrokerV2();
  void runNetFSMV2();

  // WDT helpers
  void enableWDTV2(int timeoutMS = 0);
  void feedWDTV2();

  // Error handling helpers
  void
  haltErrorV2(String error,
              ws_led_status_t ledStatusColor = WS_LED_STATUS_ERROR_RUNTIME);
  void errorWriteHangV2(String error);

  // MQTT topic callbacks //
  // Decodes a signal message
  bool decodeSignalMsgV2(
      wippersnapper_signal_v1_CreateSignalRequest *encodedSignalMsg);

  // Encodes a pin event message
  bool encodePinEventV2(
      wippersnapper_signal_v1_CreateSignalRequest *outgoingSignalMsg,
      uint8_t pinName, int pinVal);

  // Pin configure message
  bool
  configureDigitalPinReqV2(wippersnapper_pin_v1_ConfigurePinRequest *pinMsg);
  bool configAnalogInPinReqV2(wippersnapper_pin_v1_ConfigurePinRequest *pinMsg);

  // I2C
  std::vector<WipperSnapper_Component_I2C *>
      i2cComponentsV2; ///< Vector containing all I2C components
  WipperSnapper_Component_I2C *_i2cPort0V2 =
      NULL; ///< WipperSnapper I2C Component for I2C port #0
  WipperSnapper_Component_I2C *_i2cPort1V2 =
      NULL; ///< WipperSnapper I2C Component for I2C port #1
  bool _isI2CPort0InitV2 =
      false; ///< True if I2C port 0 has been initialized, False otherwise.
  bool _isI2CPort1InitV2 =
      false; ///< True if I2C port 1 has been initialized, False otherwise.

  uint8_t _bufferV2[WS_MQTT_MAX_PAYLOAD_SIZE]; /*!< Shared buffer to save
                                                callback payload */
  uint8_t
      _buffer_outgoingV2[WS_MQTT_MAX_PAYLOAD_SIZE]; /*!< buffer which contains
                                                     outgoing payload data */
  uint16_t bufSizeV2; /*!< Length of data inside buffer */

  ws_board_status_t _boardStatusV2 =
      WS_BOARD_DEF_IDLE; ///< Hardware's registration status

  // TODO: We really should look at making these static definitions, not dynamic
  // to free up space on the heap
  Wippersnapper_DigitalGPIO *_digitalGPIOV2; ///< Instance of digital gpio class
  Wippersnapper_AnalogIO *_analogIOV2;       ///< Instance of analog io class
  Wippersnapper_FS_V2 *_fileSystemV2; ///< Instance of Filesystem (native USB)
  WipperSnapper_LittleFS
      *_littleFSV2; ///< Instance of LittleFS Filesystem (non-native USB)
#ifdef USE_DISPLAY
  ws_display_driver *_displayV2 = nullptr; ///< Instance of display driver class
  ws_display_ui_helper *_ui_helperV2 =
      nullptr; ///< Instance of display UI helper class
#endif
  ws_pixels *_ws_pixelsComponentV2; ///< ptr to instance of ws_pixels class
  ws_pwm *_pwmComponentV2;          ///< Instance of pwm class
  ws_servo *_servoComponentV2;      ///< Instance of servo class
  ws_ds18x20 *_ds18x20ComponentV2;  ///< Instance of DS18x20 class
  ws_uart *_uartComponentV2;        ///< Instance of UART class

  // TODO: does this really need to be global?
  uint8_t _macAddrV2[6];  /*!< Unique network iface identifier */
  char sUIDV2[13];        /*!< Unique network iface identifier */
  const char *_boardIdV2; /*!< Adafruit IO+ board string */
  Adafruit_MQTT *_mqttV2; /*!< Reference to Adafruit_MQTT, _mqtt. */

  secretsConfig _configV2; /*!< Wippersnapper secrets.json as a struct. */
  networkConfig _multiNetworksV2[3]; /*!< Wippersnapper networks as structs. */
  bool _isWiFiMultiV2 = false; /*!< True if multiple networks are defined. */

  // TODO: Does this need to be within this class?
  int32_t totalDigitalPinsV2; /*!< Total number of digital-input capable pins */

  wippersnapper_signal_v1_CreateSignalRequest
      _incomingSignalMsgV2; /*!< Incoming signal message from broker */
  wippersnapper_signal_v1_I2CRequest msgSignalI2CV2 =
      wippersnapper_signal_v1_I2CRequest_init_zero; ///< I2C request wrapper
                                                    ///< message

  // ds signal msg
  wippersnapper_signal_v1_Ds18x20Request msgSignalDSV2 =
      wippersnapper_signal_v1_Ds18x20Request_init_zero; ///< DS request message
                                                        ///< wrapper

  // servo message
  wippersnapper_signal_v1_ServoRequest
      msgServoV2; ///< ServoRequest wrapper message
  wippersnapper_signal_v1_PWMRequest msgPWMV2 =
      wippersnapper_signal_v1_PWMRequest_init_zero; ///< PWM request wrapper
                                                    ///< message.

  // pixels signal message
  wippersnapper_signal_v1_PixelsRequest
      msgPixelsV2; ///< PixelsRequest wrapper message

  wippersnapper_signal_v1_UARTRequest
      msgSignalUARTV2; ///< UARTReq wrapper message

  char *throttleMessageV2; /*!< Pointer to throttle message data. */
  int throttleTimeV2;      /*!< Total amount of time to throttle the device, in
                            milliseconds. */

  bool pinCfgCompletedV2 = false; /*!< Did initial pin sync complete? */

// enable LEDC if esp32
#ifdef ARDUINO_ARCH_ESP32
  ws_ledc *_ledcV2 = nullptr; ///< Pointer to LEDC object
#endif

private:
  void _initV2();

protected:
  ws_status_t _statusV2 = WS_IDLE;   /*!< Adafruit IO connection status */
  uint32_t _last_mqtt_connectV2 = 0; /*!< Previous time when client connected to
                                          Adafruit IO, in milliseconds. */
  uint32_t _prv_pingV2 = 0; /*!< Previous time when client pinged Adafruit IO's
                             MQTT broker, in milliseconds. */
  uint32_t _prvKATBlinkV2 = 0; /*!< Previous time when client pinged Adafruit
                             IO's MQTT broker, in milliseconds. */

  // Device information
  const char *_deviceIdV2; /*!< Adafruit IO+ device identifier string */
  char *_device_uidV2;     /*!< Unique device identifier  */

  // MQTT topics
  char *_topicB2d;
  char *_topicD2b;
  char *_topicError;
  char *_topicThrottle;

  // Adafruit_MQTT Subscription objects
  Adafruit_MQTT_Subscribe *_subscribeB2d;
  Adafruit_MQTT_Subscribe *_subscribeError;
  Adafruit_MQTT_Subscribe *_subscribeThrottle;

  // Adafruit_MQTT Publish objects
  Adafruit_MQTT_Publish *_publishD2b;

  wippersnapper_signal_v1_CreateSignalRequest
      _outgoingSignalMsgV2; /*!< Outgoing signal message from device */
};
extern Wippersnapper_V2 WsV2; ///< Global member variable for callbacks

#endif // WIPPERSNAPPER_V2_H