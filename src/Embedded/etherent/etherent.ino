#include <EtherCard.h>
#include <AltSoftSerial.h>
// #include <DateTimeStrings.h>

#define BAUD_RATE 9600
#define DIGIT_COUNT 10
#define BUFFER_SIZE 900

static byte myip[] = { 169, 254, 238, 216 };
static byte gwip[] = { 169, 254, 238, 215 };
static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };
static byte serverip[] = { 169, 254, 238, 215 };


byte Ethernet::buffer[BUFFER_SIZE];
const char server[] PROGMEM = "localhost";
AltSoftSerial tagScanner;
Stash stash;
static byte session;

String rfid = "";
char c;
int charCount = 0;

bool readRfid(char rfidTag[]) {
  while(tagScanner.available()>0) {
    c = tagScanner.read(); 
    charCount++;
    rfid += c;
    if (charCount == DIGIT_COUNT) {
      strcpy(rfidTag, rfid.c_str());
      rfid = "";
      charCount = 0;
      return true; 
    }
  }
  return false;
}

void setup() {
  tagScanner.begin(BAUD_RATE);
  Serial.begin(BAUD_RATE);

  ether.begin(sizeof Ethernet::buffer, mymac, SS);
  ether.staticSetup(myip, gwip);
  ether.copyIp(ether.hisip, serverip);
  ether.persistTcpConnection(true);
} 

void sendToServer(char rfidTag[]) {
    byte sd = stash.create();
    stash.print("{");
    stash.print("\r\"type\": \"rfid\",");
    stash.print("\r\"data\": {");
    stash.print("\r\r\"tag\": ");
    stash.print("\"");
    for (int i = 0; rfidTag[i] != '\0'; i++) {
        stash.print(rfidTag[i]);
    }
    stash.print("\"");
    stash.print("\r}");
    stash.print("}");
    stash.save();

    Stash::prepare(PSTR("POST http://$F/ HTTP/1.1" "\r\n"
                  "Host: $F" "\r\n"
                  "Content-Length: $D" "\r\n"
                  "Content-Type: application/json" "\r\n"
                  "\r\n"
                  "$H"),
    server, server, stash.size(), sd);
    session = ether.tcpSend();
}

void receiveFromServer(char rfidTag[]) {
  char* response = ether.tcpReply(session);
  if (response != nullptr) {
    const char* statusStart = strstr(response, "HTTP/1.1 ");
    if (statusStart != NULL) {
        statusStart += strlen("HTTP/1.1 ");
        int statusCode;
        sscanf(statusStart, "%d", &statusCode);
        Serial.print(statusCode);
        Serial.print(":");
        for (int i = 0; i < DIGIT_COUNT; i++) {
          Serial.print(rfidTag[i]);
        }
    }
  }
}

void loop() {
  char rfidTag[DIGIT_COUNT];
  word pos = ether.packetLoop(ether.packetReceive());
  if (readRfid(rfidTag)) {
        sendToServer(rfidTag);
  }
  receiveFromServer(rfidTag);
}
