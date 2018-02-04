#include <aJSON.h>
#include <SPI.h>
#include <Ethernet.h>

#define ENERGIA_PLATFORM
#define M2X_ENABLE_READER
#include "M2XStreamClient.h"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,17);

char deviceId[] = "<device id>"; // Device you want to receive values
char streamName[] = "<stream name>"; // Stream you want to receive values
char m2xKey[] = "<M2X access key>"; // Your M2X access key

EthernetClient client;
M2XStreamClient m2xClient(&client, m2xKey);

void on_data_point_found(const char* at, m2x_value value, int index, void* context, int type) {
  Serial.print("Found a data point, index:");
  Serial.println(index);
  Serial.print("Type:");
  Serial.println(type);
  Serial.print("At:");
  Serial.println(at);
  Serial.print("Value:");
  switch (type) {
    case 1:
      Serial.println(value.value_string);
      break;
    case 3:
      Serial.println(value.value_integer);
      break;
    case 4:
      Serial.println(value.value_number);
      break;
  }
}

void setup() {
  Serial.begin(115200);

  Serial.print("Attempting to connect to Ethernet");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }

  // wait 10 seconds for connection:
  delay(10000);

  Serial.println("Connected to Ethernet");
  printEthernetStatus();
}

void loop() {
  int response = m2xClient.listStreamValues(deviceId, streamName, on_data_point_found, NULL);
  Serial.print("M2X client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}

void printEthernetStatus() {
  // print local IP
  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());

  // print subnet mask
  Serial.print("Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());

  // print gateway IP
  Serial.print("Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());

  // print dns IP
  Serial.print("DNS IP: ");
  Serial.println(Ethernet.dnsServerIP());
}
