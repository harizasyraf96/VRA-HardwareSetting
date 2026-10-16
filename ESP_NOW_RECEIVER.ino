#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// =====================
// OUTPUT PINS
// =====================
#define LEFT_SCREEN   18
#define RIGHT_SCREEN  19

#define LEFT_TOY      21
#define RIGHT_TOY     22

// =====================
// COMMANDS
// Must match transmitter
// =====================
#define CMD_SCREEN 1
#define CMD_TOY    2
#define CMD_BOTH   3
#define CMD_STOP   4

// =====================
// Data Structure
// Must match transmitter
// =====================
typedef struct {
  int command;
  int side;
} struct_message;

struct_message myData;

// =====================
// Utility
// =====================
void allOff()
{
  digitalWrite(LEFT_SCREEN, LOW);
  digitalWrite(RIGHT_SCREEN, LOW);

  digitalWrite(LEFT_TOY, LOW);
  digitalWrite(RIGHT_TOY, LOW);
}

// =====================
// Print MAC
// =====================
void printMacBytes(const uint8_t *mac)
{
  char buf[18];

  sprintf(buf,
          "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2],
          mac[3], mac[4], mac[5]);

  Serial.println(buf);
}

//// =====================
//// ESP-NOW Receive Callback
//// =====================
//void onDataRecv(const esp_now_recv_info_t *info,
//                const uint8_t *incomingData,
//                int len)
//{
//  memcpy(&myData,
//         incomingData,
//         sizeof(myData));
//
//  Serial.println("==========");
//  Serial.print("Command: ");
//  Serial.println(myData.command);
//
//  Serial.print("Side: ");
//  Serial.println(myData.side);
//
//  switch (myData.command)
//  {
//    case CMD_SCREEN:
//      if (myData.side == 1)
//      {
//        digitalWrite(LEFT_SCREEN, HIGH);
//        Serial.println("LEFT SCREEN ON");
//      }
//      if (myData.side == 2)
//      {
//        digitalWrite(RIGHT_SCREEN, HIGH);
//        Serial.println("RIGHT SCREEN ON");
//      }
//      break;
//    case CMD_TOY:
//      if (myData.side == 1)
//      {
//        digitalWrite(LEFT_TOY, HIGH);
//        Serial.println("LEFT TOY ON");
//      }
//      if (myData.side == 2)
//      {
//        digitalWrite(RIGHT_TOY, HIGH);
//        Serial.println("RIGHT TOY ON");
//      }
//      break;
//    case CMD_BOTH:
//      if (myData.side == 1)
//      {
//        digitalWrite(LEFT_SCREEN, HIGH);
//        digitalWrite(LEFT_TOY, HIGH);
//        Serial.println("LEFT SCREEN + TOY ON");
//      }
//      if (myData.side == 2)
//      {
//        digitalWrite(RIGHT_SCREEN, HIGH);
//        digitalWrite(RIGHT_TOY, HIGH);
//        Serial.println("RIGHT SCREEN + TOY ON");
//      }
//      break;
//    case CMD_STOP:
//      allOff();
//      Serial.println("STOP");
//      break;
//  }
//}


void onDataRecv(const esp_now_recv_info_t *info,
                const uint8_t *incomingData,
                int len)
{
  memcpy(&myData,
         incomingData,
         sizeof(myData));

  Serial.println("==========");
  Serial.print("Command: ");
  Serial.println(myData.command);

  Serial.print("Side: ");
  Serial.println(myData.side);
}


// =====================
// Setup
// =====================
void setup()
{
  Serial.begin(115200);
  delay(1500);
  pinMode(LEFT_SCREEN, OUTPUT);
  pinMode(RIGHT_SCREEN, OUTPUT);
  pinMode(LEFT_TOY, OUTPUT);
  pinMode(RIGHT_TOY, OUTPUT);
  allOff();
  WiFi.mode(WIFI_STA);
  delay(200);
  Serial.print("WiFi.macAddress(): ");
  Serial.println(WiFi.macAddress());
  uint8_t mac[6];
  esp_err_t r =
      esp_wifi_get_mac(WIFI_IF_STA,
                       mac);
  Serial.print("esp_wifi_get_mac(): ");
  Serial.println(r == ESP_OK ?
                 "OK" :
                 String((int)r));
  Serial.print("MAC bytes: ");
  printMacBytes(mac);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW Init Failed");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
  Serial.println("Receiver Ready");
}

// =====================
// Loop
// =====================
void loop()
{
  delay(100);
}
