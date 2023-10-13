/* Mi-ichi
 by Asterki (https://www.asterki.com)
 
 modified 11 Oct 2023
 by Fernando Rivera
 
 board used
 WEMOS LOLIN32 Lite

 Libraries used
 Interval
 AnalogWrite
*/
 
// Libs
#include "BluetoothSerial.h"
#include <pwmWrite.h>
 
 
// BT Config
String device_name = "Mi-ichi";
BluetoothSerial SerialBT;
 

// Proximity Sensor Config
const int triggerPin = 23;
const int echoPin = 22;
long duration, cm;


int walking = 1;
 
// Legs, read documentation to know about each pin
Pwm pwm = Pwm();


// Preset Positions                   fr            br            bl            fl
// Legend:                        s   a   p     s   a   p     s   a   p     s   a   p
const byte servoPosStandUp[12] = {90, 110, 110,   90, 110, 110,  90, 70, 70,  90, 70, 70};
const byte servoPosLayDown[12] = {90, 180, 90,  90, 30, 0,  90, 150, 210, 90, 0, 90};
const byte servoPosStretch[12] = {90, 180, 90,  90, 60, 60,   90, 120, 120, 90, 0, 90};
const byte servoPosSitDown[12] = {90, 130, 90,   90, 40, 260,    90, 140, 220,  90, 50, 90};
 
const byte servoPins[12] =       {16, 15, 17,   18, 19, 14,   27, 32, 33,   23, 25, 26};


BT Event Listener
void BT_EventHandler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_DATA_IND_EVT ) {
    while (SerialBT.available()) {
      int incoming = SerialBT.read();
 
      if (incoming == 49) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosStandUp[i]);
            walking = 0;
        }
      }
      
      if (incoming == 50) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosSitDown[i]);
            walking = 0;
        }
      }
      
      if (incoming == 51) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosLayDown[i]);
            walking = 0;
        }
      }
      
      if (incoming == 52) {
        for (int i = 0; i <= 12; i++) {
          pwm.writeServo(servoPins[i], servoPosStretch[i]);
            walking = 0;
        }
      }
 
      if (incoming == 53) {
        walking = 1;
      }      
      if (incoming == 54) {
        walking = 0;
      } 
    }
  } 
}


 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
 
  // Set all servos to initial position
  for (int i = 0; i <= 12; i++) {
    pwm.writeServo(servoPins[i], servoPosStandUp[i]);
  }
 
  // Start BT
  SerialBT.begin(device_name);
  SerialBT.register_callback(BT_EventHandler);
  Serial.println("[INFO] - Bluetooth device started");
}

int walkingMotion[2][12] = {
      {90, 160, 170, 90, 130, 90, 90, 20, 10, 90, 50, 90},
      {90, 50, 90, 90, 20, 10, 90, 130, 90, 90, 20, 10}
};

int counter = 10;

void loop() {
  if (walking == 1) {
    // Arms
    pwm.writeServo(15, 80); // Front right 
    pwm.writeServo(19, 130); // Back right 
    pwm.writeServo(25, 130); // Front left 
    pwm.writeServo(32, 160); // Back left 

    // Paw
    pwm.writeServo(17, 10); // Front right 
    pwm.writeServo(14, 90); // Back right 
    pwm.writeServo(26, 90); // Front left 
    pwm.writeServo(33, 170); // Back left 
    
    delay(350);
  
    pwm.writeServo(15, 120); // Front right 
    pwm.writeServo(19, 160); // Back right 
    pwm.writeServo(25, 160); // Front left 
    pwm.writeServo(32, 50); // Back left 

    // Paw
    pwm.writeServo(17, 90); // Front right 
    pwm.writeServo(14, 170); // Back right 
    pwm.writeServo(26, 170); // Front left 
    pwm.writeServo(33, 90); // Back left 
    delay(350);
  } else if (walking == 2) { // Walk backwards
    // Arms
    pwm.writeServo(15, 160); // Front right 
    pwm.writeServo(19, 50); // Back right 
    pwm.writeServo(25, 50); // Front left 
    pwm.writeServo(32, 20); // Back left 

    // Paw
    pwm.writeServo(17, 170); // Front right 
    pwm.writeServo(14, 90); // Back right 
    pwm.writeServo(26, 90); // Front left 
    pwm.writeServo(33, 10); // Back left 
    
    delay(350);
  
    pwm.writeServo(15, 130); // Front right 
    pwm.writeServo(19, 20); // Back right 
    pwm.writeServo(25, 20); // Front left 
    pwm.writeServo(32, 130); // Back left 

    // Paw
    pwm.writeServo(17, 90); // Front right 
    pwm.writeServo(14, 10); // Back right 
    pwm.writeServo(26, 10); // Front left 
    pwm.writeServo(33, 90); // Back left 
    delay(350);
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