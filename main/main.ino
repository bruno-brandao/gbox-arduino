#include <EtherCard.h>

static uint32_t timer;
const char website[] PROGMEM = "192.168.0.17";
char temp_sensor[150];
int an0 = A0;
int sensor = 0;

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];

static void my_callback (byte status, word off, word len)
{
  Serial.println(">>>");
  Ethernet::buffer[off + len] = 0;
  char* fullResponse;
  fullResponse = (char*) Ethernet::buffer + off;
  String str = String(fullResponse);
  int checkResp = 0;
  checkResp = str.indexOf(":true");
  Serial.println(checkResp);
  if (checkResp != -1) {
    char json[str.length()];
    str.toCharArray(json, str.length() + 1);
    Serial.println(json);
  }
}

void setup () {
  Serial.begin(9600);
  Serial.println("Obtendo IP:");
  Serial.println(F("\n[testDHCP]"));

  Serial.print("MAC: ");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mymac[i], HEX);
    if (i < 5)
      Serial.print(':');
  }
  Serial.println();

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  Serial.println(F("Setting up DHCP"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);
//  if (!ether.dnsLookup(website))     //IP do servidor por DNS
//    Serial.println("Falha no DNS");
  ether.parseIp(ether.hisip, "192.168.0.17"); //IP do servidor manual
  ether.printIp("Servidor: ", ether.hisip);
  Serial.println(" ");
}

void loop ()
{
  ether.packetLoop(ether.packetReceive());
  if (millis() > timer)
  {
    timer = millis() + 5000;
    ether.persistTcpConnection(true);
    sensor = analogRead(an0);
    sprintf(temp_sensor, "?val=%d", sensor); 
    ether.hisport = 8080;//to access  local host
    ether.browseUrl(PSTR("/gbox/9"), temp_sensor, website, my_callback); 
  }
}
