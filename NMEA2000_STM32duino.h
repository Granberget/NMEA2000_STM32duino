#ifndef _NMEA2000_STM32DUINO_H_
#define _NMEA2000_STM32DUINO_H_

#include <NMEA2000.h>
#include <N2kMsg.h>
#include "STM32_CAN.h"  // Ditt CAN-bibliotek för STM32

/*!
  \brief En implementering av NMEA2000 CAN-gränssnittet för STM32duino

  Denna klass ärver från tNMEA2000 och överskuggar de virtuella metoderna
  som hanterar CAN-öppning, sändning och mottagning med hjälp av STM32_CAN-biblioteket.
*/
class tNMEA2000_STM32duino : public tNMEA2000 {
public:
  /// Konstruktor
  tNMEA2000_STM32duino();

protected:
  /// Initierar interna buffertar (kan anpassas om det behövs)
  virtual void InitCANFrameBuffers();

  /// Öppnar CAN-gränssnittet
  virtual bool CANOpen();

  /// Skickar en CAN-ram med givet id, längd och databyte-array.
  virtual bool CANSendFrame(unsigned long id, unsigned char len, const unsigned char *buf, bool wait_sent = true);

  /// Hämtar en CAN-ram om en finns tillgänglig.
  virtual bool CANGetFrame(unsigned long &id, unsigned char &len, unsigned char *buf);
};

#endif
