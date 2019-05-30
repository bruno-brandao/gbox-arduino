#include <EtherCard.h>

// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 192,168,0,23 };

byte Ethernet::buffer[500];

BufferFiller bfill;

static homePage(){
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR("Ola mundo!"));

  return bfill.position();
}

void setup () {
  Serial.begin(9600);
  Serial.println("\n[getViaDNS]");

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, 53) == 0)
    Serial.println( "Failed to access Ethernet controller");
  else
    Serial.println( "Ethernet controller success");

  ether.staticSetup(myip);
//
//  if (!ether.dnsLookup(website))
//    Serial.println("DNS failed");
//  ether.printIp("Server: ", ether.hisip);
}

void loop () {
  word pos = ether.packetLoop(ether.packetReceive());
  Serial.println("success");
//
  if (pos) {
    ether.httpServerReply(homePage());
  }
}
