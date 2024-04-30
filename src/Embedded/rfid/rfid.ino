#include <EtherCard.h>
#include <LiquidCrystal.h>
#include <AltSoftSerial.h>
#include <String.h>

#define GREEN_LED 13
#define RED_LED 10
#define MOTOR_IN1 7
#define MOTOR_IN2 6
#define DELAY 3000
#define DELAY_RED_LED 3000
#define BAUD_RATE 9600
#define DIGIT_COUNT 10
#define DELAY_FOR_90_DEGREES_CLOCKWISE 95
#define DELAY_FOR_90_DEGREES_COUNTERCLOCKWISE 82
#define SUCCESS_CODE 200
#define FAILURE_CODE 403
#define ACCESS_DENIED "Access Denied!"

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
AltSoftSerial resPrinter;

struct Status {
  bool isRedLedOn;
  bool isGreenLedOn;
  bool isPrintedOnMonitor;
  bool isDoorOpen;

  unsigned long lastTimeRedLedOn;
  unsigned long lastTimeGreenLedOn;
  unsigned long lastTimeMonitorPrinted;
  unsigned long lastTimeDoorOpen;
};

Status currentStatus = {
  false,
  false,
  false,
  false,
  0,
  0,
  0,
  0
};

void setup() {
  Serial.begin(BAUD_RATE);
  resPrinter.begin(BAUD_RATE);
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  lcd.begin(20, 4);
  lcd.print("Scan your TAG");
}

String result = "";
char c;
int charCount = 0;

bool readRfid(char res[]) {
  while(Serial.available()>0) {
    c = Serial.read(); 
    charCount++;
    result += c;
    if (charCount == 14) {
      strcpy(res, result.c_str());
      result = "";
      charCount = 0;
      return true; 
    }
  }
  return false;
}

void turnOnRedLed() {
  if (currentStatus.isGreenLedOn) {
    turnOffGreenLed();
  }
  digitalWrite(RED_LED, HIGH);
  currentStatus.isRedLedOn = true;
  currentStatus.lastTimeRedLedOn = millis();
}

void turnOffRedLed() {
  digitalWrite(RED_LED, LOW);
  currentStatus.isRedLedOn = false;
}

void turnOnGreenLed() {
  if (currentStatus.isRedLedOn) {
    turnOffRedLed();
  }
  digitalWrite(GREEN_LED, HIGH);
  currentStatus.isGreenLedOn = true;
  currentStatus.lastTimeGreenLedOn = millis();
}

void turnOffGreenLed() {
  digitalWrite(GREEN_LED, LOW);
  currentStatus.isGreenLedOn = false;
}



void openDoor() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  currentStatus.isDoorOpen = true;
  currentStatus.lastTimeDoorOpen = millis();
  delay(DELAY_FOR_90_DEGREES_CLOCKWISE);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
}

void closeDoor() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  currentStatus.isDoorOpen = false;
  delay(DELAY_FOR_90_DEGREES_COUNTERCLOCKWISE);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
}

void checkStatus() {
  long unsigned int currentTime = millis();
  if (currentStatus.isRedLedOn && currentTime - currentStatus.lastTimeRedLedOn >= DELAY_RED_LED) {
    turnOffRedLed();
  }
  if (currentStatus.isGreenLedOn && currentTime - currentStatus.lastTimeGreenLedOn >= DELAY) {
    turnOffGreenLed();
  }
  if (currentStatus.isDoorOpen && currentTime - currentStatus.lastTimeDoorOpen >= DELAY) {
    closeDoor();
  }
  if (currentStatus.isPrintedOnMonitor && currentTime - currentStatus.lastTimeMonitorPrinted >= DELAY) {
    lcd.clear();
    currentStatus.isPrintedOnMonitor = false;
  }
}

void accessGranted(char rfid[]) {
  lcd.setCursor(0, 1);
  lcd.print(rfid);
  currentStatus.isPrintedOnMonitor = true;
  currentStatus.lastTimeMonitorPrinted = millis();
  turnOnGreenLed();
  openDoor();
}

void accessDenied() {
  lcd.setCursor(0, 1);
  lcd.print(ACCESS_DENIED);
  currentStatus.isPrintedOnMonitor = true;
  currentStatus.lastTimeMonitorPrinted = millis();
  turnOnRedLed();
  if (currentStatus.isDoorOpen) {
      lcd.clear();
      closeDoor();
  }
}

char res[15];
int statusCode;
char rfid[11];
char status[4];

void loop() {
  if (readRfid(res)) {
    lcd.clear();
    strcpy(status, res);
    status[3] = '\0'; 
    strcpy(rfid, res);
    resPrinter.println(status);
    resPrinter.println(rfid);
    statusCode = atoi(status);
    if (statusCode == SUCCESS_CODE) {
      accessGranted(rfid);
    }
    else if (statusCode == FAILURE_CODE) {
      accessDenied();
    }
    memset(res, '\0', sizeof(res));
    memset(rfid, '\0', sizeof(rfid));
  }
  checkStatus();
}

