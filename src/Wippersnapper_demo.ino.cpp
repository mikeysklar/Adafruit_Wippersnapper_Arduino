# 1 "/var/folders/ff/dmzflvf52tq9kzvt6g8jglxw0000gn/T/tmpsdf3og7x"
#include <Arduino.h>
# 1 "/Users/brentrubell/Documents/Arduino/libraries/Adafruit_Wippersnapper_Arduino/src/Wippersnapper_demo.ino"
# 16 "/Users/brentrubell/Documents/Arduino/libraries/Adafruit_Wippersnapper_Arduino/src/Wippersnapper_demo.ino"
#include "Wippersnapper_Networking.h"
Wippersnapper_WiFi wipper;


#define WS_DEBUG
void setup();
void loop();
#line 22 "/Users/brentrubell/Documents/Arduino/libraries/Adafruit_Wippersnapper_Arduino/src/Wippersnapper_demo.ino"
void setup() {

  wipper.provision();

  Serial.begin(115200);
  while (!Serial) delay(10);

  wipper.connect();

}

void loop() {
  wipper.run();
}