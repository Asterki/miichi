/* Mi-ichi
 by Asterki (https://www.asterki.com)
 
 modified 14 Oct 2023
 by Fernando Rivera
 
 board used
 WEMOS LOLIN32 Lite

 Libraries used
 Interval
 AnalogWrite
*/
 
// Libs
//#include "BluetoothSerial.h"
#include <pwmWrite.h>
#include <iterator> 
 
// BT Config
// String device_name = "Mi-ichi";
// BluetoothSerial SerialBT;
 

// Proximity Sensor Config
const int triggerPin = 23;
const int echoPin = 22;
long duration, cm;


int currentMotion = 1;
 
// Legs, read documentation to know about each pin
Pwm pwm = Pwm();


// Preset Positions                   fr            br            bl            fl
// Legend:                        s   a   p     s   a   p     s   a   p     s   a   p
const byte servoPosStandUp[12] = {90, 110, 110, 90, 110, 110, 90, 70, 70,   90, 70, 70};
const byte servoPosLayDown[12] = {90, 180, 90,  90, 30, 0,    90, 150, 210, 90, 0, 90};
const byte servoPosStretch[12] = {90, 180, 90,  90, 60, 60,   90, 120, 120, 90, 0, 90};
const byte servoPosSitDown[12] = {90, 130, 90,  90, 40, 260,  90, 140, 220, 90, 50, 90};

// Preset motions
int motionSpeed = 350;
const byte motionWalk[2][12] = {
  {90, 140, 130,  90, 90, 90,  90, 140, 130,  90, 90, 90},
  {90, 90,  90,   90, 60, 60,  90, 90,  90,   90, 60, 60}
};
const byte motionBackwardsWalk[2][12] = {
  {90, 160, 170,  90, 50, 90,   90, 20, 10,    90, 50, 90},
  {90, 130,  90,  90, 160, 170, 90, 130, 90,   90, 20, 10}
};
 
const byte servoPins[12] =       {16, 15, 17,   18, 19, 14,   27, 32, 33,   23, 25, 26};


// BT Event Listener
/*void BT_EventHandler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_DATA_IND_EVT ) {
    while (SerialBT.available()) {
      int incoming = SerialBT.read();
 
      if (incoming == 49) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosStandUp[i]);
            currentMotion = 0;
        }
      }
      
      if (incoming == 50) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosSitDown[i]);
            currentMotion = 0;
        }
      }
      
      if (incoming == 51) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosLayDown[i]);
            currentMotion = 0;
        }
      }
      
      if (incoming == 52) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosStretch[i]);
            currentMotion = 0;
        }
      }
 
      if (incoming == 53) {
        currentMotion = 1;
      }
      if (incoming == 54) {
        currentMotion = 2;
      }      
      if (incoming == 55) {
        currentMotion = 0;
      } 
    }
  } 
}*/

 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
 
  // Set all servos to initial position
  for (int i = 0; i < 12; i++) {
    pwm.writeServo(servoPins[i], servoPosStandUp[i]);
  }
 
  // Start BT
  // SerialBT.begin(device_name);
  // SerialBT.register_callback(BT_EventHandler);
  Serial.println("[INFO] - Bluetooth device started");
}


void loop() {
  if (currentMotion == 1) {
    for (int j = 0; j < 2; j++) {
      for (int i = 0; i < 12; i++) {
        pwm.writeServo(servoPins[i], motionWalk[j][i]);
      }
      delay(motionSpeed);
    }
  } else if (currentMotion == 2) { // Walk backwards
    for (int j = 0; j < 2; j++) {
      for (int i = 0; i < 12; i++) {
        pwm.writeServo(servoPins[i], motionBackwardsWalk[j][i]);
      }
      delay(motionSpeed);
    }
  }
  
  /*
  digitalWrite(triggerPin, LOW);
  digitalWrite(triggerPin, HIGH);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1; */  

  // Serial.println(cm);
}
