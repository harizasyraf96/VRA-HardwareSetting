#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMac[] = {0x80, 0x7D, 0x3A, 0xD1, 0x2B, 0xAC};

#define LED_RIGHT 2
#define LED_LEFT 4
#define LED_R_SCREEN 5
#define LED_L_SCREEN 18
#define LED_R_TOY 19
#define LED_L_TOY 21
#define LED_STOP 23

#define BTN_LEFT 27
#define BTN_RIGHT 26
#define BTN_SCREEN 25
#define BTN_TOY 33
#define BTN_BOTH 32
#define BTN_STOP 14

// Commands
#define CMD_SCREEN 1
#define CMD_TOY 2
#define CMD_BOTH 3
#define CMD_STOP 4


typedef struct {
  int command;
  int side;
  int button;
  int value;
} struct_message;

struct_message myData;
int selectedSide = 0;  //0=none, 1=left, 2=right

// ESP32 Core 3.x callback
void OnDataSent(const wifi_tx_info_t *tx_info,
                esp_now_send_status_t status)
{
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setupLEDs()
{
  pinMode(LED_RIGHT, OUTPUT);
  pinMode(LED_LEFT, OUTPUT);
  pinMode(LED_R_SCREEN, OUTPUT);
  pinMode(LED_L_SCREEN, OUTPUT);
  pinMode(LED_R_TOY, OUTPUT);
  pinMode(LED_L_TOY, OUTPUT);
  pinMode(LED_STOP, OUTPUT);
}

void setupButtons()
{
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_SCREEN, INPUT_PULLUP);
  pinMode(BTN_TOY, INPUT_PULLUP);
  pinMode(BTN_BOTH, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
}

void selectLeft()
{
  selectedSide = 1;
  digitalWrite(LED_LEFT, HIGH);
  digitalWrite(LED_RIGHT, LOW);
  Serial.println("LEFT Selected");
}

void selectRight()
{
  selectedSide * 2;
  digitalWrite(LED_RIGHT, HIGH);
  digitalWrite(LED_LEFT, LOW);
  Serial.println("RIGHT Selected");
}

void stopAll()
{
  digitalWrite(LED_RIGHT, LOW);
  digitalWrite(LED_LEFT, LOW);
  digitalWrite(LED_R_SCREEN, LOW);
  digitalWrite(LED_L_SCREEN, LOW);
  digitalWrite(LED_R_TOY, LOW);
  digitalWrite(LED_L_TOY, LOW);
  digitalWrite(LED_STOP, LOW);
}

void sendCommand(int command, int side) {
  myData.command = command;
  myData.side = side;

  esp_err_t result =
    esp_now_send(receiverMac,
                 (uint8_t *)&myData,
                 sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Command Sent");
  } else {
    Serial.println("Send Error");
  }
}



void setup() {
  Serial.begin(115200);
  setupLEDs();
  setupButtons();
  stopAll();

  // pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  Serial.print("My MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
//  if (digitalRead(BUTTON_PIN) == LOW) {
//    myData.button = 1;
//    myData.value = 0;
//
//    esp_now_send(receiverMac,
//                 (uint8_t *)&myData,
//                 sizeof(myData));
//    delay(200); // debounce
//    //  if (result == ESP_OK) {
//    //    Serial.println("Sending...");
//    //  } else {
//    //    Serial.println("Error sending");
//    //  }
//  }

if (digitalRead(BTN_LEFT) == LOW) {
  selectLeft();
  selectedSide = 1;
  delay(200);
}

if (digitalRead(BTN_RIGHT) == LOW) {
  selectRight();
  selectedSide = 2;
  delay(200);
}

if (selectedSide == 1) {
    if (digitalRead(BTN_SCREEN) == LOW) {
      sendCommand(CMD_SCREEN, 1);
      digitalWrite(LED_L_SCREEN, HIGH);
      Serial.println("LEFT SCREEN");
      while (digitalRead(BTN_SCREEN) == LOW) {
        delay(10);
      }
    }
    if (digitalRead(BTN_TOY) == LOW) {
      sendCommand(CMD_TOY, 1);
      digitalWrite(LED_L_TOY, HIGH);
      Serial.println("LEFT TOY");
      while (digitalRead(BTN_TOY) == LOW) {
        delay(10);
      }
    }
    if (digitalRead(BTN_BOTH) == LOW) {
      sendCommand(CMD_BOTH, 1);
      digitalWrite(LED_L_SCREEN, HIGH);
      digitalWrite(LED_L_TOY, HIGH);
      Serial.println("LEFT BOTH"); 
      while (digitalRead(BTN_BOTH) == LOW) {
        delay(10);
      }     
    }
    if (digitalRead(BTN_STOP) == LOW) {
      sendCommand(CMD_STOP, 0);
      stopAll();
      selectedSide = 0;
      Serial.println("STOP");
      while (digitalRead(BTN_STOP) == LOW) {
        delay(10);  
      }    
    }
}

if (selectedSide == 2) {
    if (digitalRead(BTN_SCREEN) == LOW) {
      sendCommand(CMD_SCREEN, 2);
      digitalWrite(LED_R_SCREEN, HIGH);
      Serial.println("RIGHT SCREEN");
      while (digitalRead(BTN_SCREEN) == LOW) {
        delay(10);      
      }
    }
    if (digitalRead(BTN_TOY) == LOW) {
      sendCommand(CMD_TOY, 2);
      digitalWrite(LED_R_TOY, HIGH);
      Serial.println("RIGHT TOY");
      while (digitalRead(BTN_TOY) == LOW) {
        delay(10);      
      }
    }
    if (digitalRead(BTN_BOTH) == LOW) {
      sendCommand(CMD_BOTH, 2);
      digitalWrite(LED_R_SCREEN, HIGH);
      digitalWrite(LED_R_TOY, HIGH);
      Serial.println("RIGHT BOTH");  
      while (digitalRead(BTN_BOTH) == LOW) {
        delay(10);      
      }    
    }
    if (digitalRead(BTN_STOP) == LOW) {
      sendCommand(CMD_STOP, 0);
      stopAll();
      selectedSide = 0;
      Serial.println("STOP");
      while (digitalRead(BTN_STOP) == LOW) {
        delay(10);      
      }
    }
}
}
