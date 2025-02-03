#include "NMEA2000_STM32duino.h"
#include "STM32_CAN.h"   // Include STM32 CAN-lib
#include <string.h>      // For memcpy

// Skapa en statisk instans av STM32_CAN.
// Justera parametrarna (exempelvis CAN1, ALT) efter din hårdvarukonfiguration.
// Alternativt kan du ändra konstruktionen så att dessa parametrar anges utifrån.
static STM32_CAN _can(CAN1, ALT);

///////////////////////////////////////////////////////////////////////////////
// Konstruktor
///////////////////////////////////////////////////////////////////////////////
tNMEA2000_STM32duino::tNMEA2000_STM32duino() {
  // Här kan du utföra eventuell konstruktor-initialisering.
  // Exempelvis: Initiera interna buffertstorlekar.
  // Den överordnade klassens InitCANFrameBuffers() anropas senare vid CAN-öppning.
}

///////////////////////////////////////////////////////////////////////////////
// InitCANFrameBuffers
///////////////////////////////////////////////////////////////////////////////
void tNMEA2000_STM32duino::InitCANFrameBuffers() {
  // Exempel på anpassning av buffertstorlekar (du kan justera dessa värden efter behov):
  if (MaxCANReceiveFrames < 10)
    MaxCANReceiveFrames = 50;
  if (MaxCANSendFrames < 10)
    MaxCANSendFrames = 40;

  // Anropa basklassens implementation (om det behövs)
  tNMEA2000::InitCANFrameBuffers();
}

///////////////////////////////////////////////////////////////////////////////
// CANOpen
///////////////////////////////////////////////////////////////////////////////
bool tNMEA2000_STM32duino::CANOpen() {
  // Initiera CAN med ditt STM32_CAN-bibliotek.
  _can.begin();
  // Ställ in baudrate – justera värdet om det behövs (här 500 kbps).
  _can.setBaudRate(500000);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// CANSendFrame
///////////////////////////////////////////////////////////////////////////////
bool tNMEA2000_STM32duino::CANSendFrame(unsigned long id, unsigned char len, const unsigned char *buf, bool /*wait_sent*/) {
  // Skapa en CAN-message-struktur enligt STM32_CAN-bibliotekets API.
  CAN_message_t msg;
  msg.id = id;
  msg.len = len;
  // Kopiera databyten (se till att len inte överskrider 8)
  memcpy(msg.buf, buf, len);

  // Make sure to use extended frame (as NMEA2000 standard)
  msg.flags.extended = 1;

  // Skicka meddelandet. Funktionen returnerar förhoppningsvis true vid lyckad sändning.
  return _can.write(msg);
}

///////////////////////////////////////////////////////////////////////////////
// CANGetFrame
///////////////////////////////////////////////////////////////////////////////
bool tNMEA2000_STM32duino::CANGetFrame(unsigned long &id, unsigned char &len, unsigned char *buf) {
  CAN_message_t msg;
  // Try to read CAN-message. If available, return true.
  if (_can.read(msg)) {
    id = msg.id;
    len = msg.len;
    memcpy(buf, msg.buf, len);
    return true;
  }
  return false;
}
