/* Mi-ichi
 by Asterki (https://www.asterki.com)
 
 modified 15 Oct 2023
 by Fernando Rivera
 
 board used
 WEMOS LOLIN32 Lite

 Libraries used
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



// Legs, read documentation to know about each pin
Pwm pwm = Pwm();
const byte servoPins[12] =       {16, 15, 17,   18, 19, 14,   27, 32, 33,   23, 25, 26};


// Preset Positions                   fr            br             bl             fl
// Legend:                        s   a   p     s   a   p      s   a   p      s   a   p
const byte servoPosStandUp[12] = {90, 70,  50,  90, 110, 130,  90, 70,  50,   90, 110, 130};
const byte servoPosLayDown[12] = {90, 180, 90,  90, 30,  0,    90, 150, 210,  90, 0, 90};
const byte servoPosStretch[12] = {90, 180, 90,  90, 60,  60,   90, 120, 120,  90, 0, 90};
const byte servoPosPooping[12] = {90, 90, 80,   90, 30, 30,    90, 150, 150,  90, 90, 110};
const byte servoPosSitDown[12] = {90, 90, 80,   90, 90, 180,   90, 90, 0,     90, 90, 110};


// Preset motions
int motionSpeed = 350;
int currentMotion = 0;
const byte motionWalk[2][12] = {
  {90, 140, 130,  90, 90, 90,  90, 140, 130,    90, 90, 90},
  {90, 90,  90,   90, 60, 60,  90, 90,  90,     90, 60, 60}
};

const byte motionWalkLeft[2][12] = {
  {90, 140, 130,  90, 90, 110,  90, 140, 130,   100, 90, 90},
  {90, 90,  90,   90, 60, 60,   90, 90,  90,    70, 60, 60}
};

const byte motionWalkRight[2][12] = {
  {90, 140, 130,  90, 90, 110,  90, 140, 130,   90, 90, 90},
  {80, 90,  90,   90, 60, 60,   90, 90,  90,    90, 60, 60}
};

const byte motionBackwardsWalk[2][12] = {
  {90, 160, 170,  90, 50, 90,     90, 20, 10,   90, 50, 90},
  {90, 130, 90,   90, 160, 170,   90, 130, 90,  90, 20, 10}
};

const byte saluteMotion[8][12] = {
  {90, 90, 80,    90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 180, 30,   90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 180, 150,  90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 180, 30,   90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 180, 150,  90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 180, 30,   90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 180, 150,  90, 90, 180,    90, 90, 0,    90, 90, 110},
  {90, 90, 80,    90, 90, 180,    90, 90, 0,    90, 90, 110},
};
 
const byte turnAround[2][12] = {
  {100, 70,  50,  100, 110, 130,  100, 70, 50,  100, 110, 130},
  {80, 70,  50,   80, 110, 130,   80, 70,  50,  80, 110, 130},
};



// BT Event Listener
void BT_EventHandler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_DATA_IND_EVT ) {
    while (SerialBT.available()) {
      int incoming = SerialBT.read();
      switch(incoming) {
        case 65: // Stand up
          for (int i = 0; i < 12; i++) {
            currentMotion = 0;
            pwm.writeServo(servoPins[i], servoPosStandUp[i]);
          }
          break;
        
        case 66: // Lay down
          for (int i = 0; i < 12; i++) {
            pwm.writeServo(servoPins[i], servoPosLayDown[i]);
              currentMotion = 0;
          }
          break;

        case 67: // Stretch
          for (int i = 0; i < 12; i++) {
            pwm.writeServo(servoPins[i], servoPosStretch[i]);
              currentMotion = 0;
          }
          break;

        case 68: // Poop
          for (int i = 0; i < 12; i++) {
            pwm.writeServo(servoPins[i], servoPosPooping[i]);
              currentMotion = 0;
          }
          break;

        case 69: // Sit down
          for (int i = 0; i < 12; i++) {
            pwm.writeServo(servoPins[i], servoPosSitDown[i]);
              currentMotion = 0;
          }
          break;

        case 70: // Salute
          currentMotion=0;
        
          for (int j = 0; j < 8; j++) {
            for (int i = 0; i < 12; i++) {
              pwm.writeServo(servoPins[i], saluteMotion[j][i]);
            }
            delay(motionSpeed);
          }
    
          delay(1000);
          currentMotion=0;

          for (int i = 0; i < 12; i++) {
            pwm.writeServo(servoPins[i], servoPosStandUp[i]);
          }
          break;

        case 71: // Walk
          currentMotion = 1;
          break;

        case 72: // Walk Backwards
          currentMotion = 2;
          break;

        case 73: // Walk Left
          currentMotion = 3;
          break;

        case 74: // Walk Right
          currentMotion = 4;
          break;

        case 75: // Dance
          currentMotion = 5;
          break;

        case 76: // Stop all motions
          currentMotion = 0;
          break;
      } 
    }
  } 
}

 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
 
  // Set all servos to initial position
  for (int i = 0; i < 12; i++) {
    pwm.writeServo(servoPins[i], servoPosSitDown[i]);
  }
 
  // Start BT
  SerialBT.begin(device_name);
  SerialBT.register_callback(BT_EventHandler);
  Serial.println("[INFO] - Bluetooth device started");
}


void loop() {
  digitalWrite(triggerPin, LOW);
  digitalWrite(triggerPin, HIGH);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1; 

  if (cm < 30) {
    currentMotion=6;
  }

  switch(currentMotion) {
    case 1: // Walk
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
          pwm.writeServo(servoPins[i], motionWalk[j][i]);
        }
        delay(motionSpeed);
      }
      break;
    
    case 2: // Walk backwards
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
          pwm.writeServo(servoPins[i], motionBackwardsWalk[j][i]);
        }
        delay(motionSpeed);
      }
      break;

    case 3: // Walk Left
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
          pwm.writeServo(servoPins[i], motionWalkLeft[j][i]);
        }
        delay(motionSpeed);
      }
      break;

    case 4: // Walk Right
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
          pwm.writeServo(servoPins[i], motionWalkRight[j][i]);
        }
        delay(motionSpeed);
      }
      break;

    case 5: // Dance
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
          pwm.writeServo(servoPins[i], motionBackwardsWalk[j][i]);
        }
        delay(motionSpeed);
      }
      break;

    case 6: // Turn around after getting too close to a wall
      for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
          pwm.writeServo(servoPins[i], turnAround[j][i]);
        }
        delay(motionSpeed);
      }
      if (cm < 30) {
        currentMotion=0;
      }
      break;
  }
}
