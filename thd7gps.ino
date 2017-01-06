/*
  thd7gps - a small ...
  Copyright (C) 2017 Paolo Mattiolo IN3AQK
  All rights reserved.

  This software is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/



#include <SoftwareSerial.h>
#include "defines.h"


char inByte;
SoftwareSerial Radio(RX, TX); // RX, TX
String nmeaLine = "";

void setup() {

  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Radio.begin(RADIO_SPEED);

}

void loop() {
  if (Radio.available()) {
    inByte = Radio.read();

    if (inByte == '\n') {
      String x = convertNmea(nmeaLine);
      Serial.println(x);
      nmeaLine = "";
    }
    else
    {
      nmeaLine += inByte;
    }
  }
}

String convertNmea (String inNmea183) {

  String outNmea182 = "";
  int gprmcLatIndex = 4;
  int gprmcLonIndex = 6;
  int gpggaLatIndex = 2;
  int gpggaLonIndex = 4;

  if (inNmea183.startsWith("$GPRMC") || inNmea183.startsWith("$GPGGA")  ) {
    int index = 0;
    int countComma = 0;
    int positionInit = 0;

    while (true) {

      outNmea182 += inNmea183[index];

      if (inNmea183.startsWith("$GPRMC") && inNmea183[index] == ',') {
        countComma += 1;
        if (countComma == gprmcLatIndex || countComma == gprmcLonIndex) {
          outNmea182[index - 1] = '@';
        }

      }
      if (inNmea183.startsWith("$GPGGA") && inNmea183[index] == ',') {
        if (countComma == gpggaLatIndex || countComma == gpggaLonIndex) {
          outNmea182[index - 1] = '@';
        }
        countComma += 1;
      }
      if (inNmea183[index] == '*') {
        break;
      }
      index++;
    }

    String cleanNemea = "";
    int chk;
    // Remove the @ marker
    for (int index = 0;  index < outNmea182.length(); index++) {
      if (outNmea182[index] != '@') {
        cleanNemea += outNmea182[index];
        if (outNmea182[index] != '@'  && outNmea182[index] != '$' && outNmea182[index] != '*') {
          chk ^= outNmea182[index];
        }
      }
    }
    
    cleanNemea += String(chk, HEX);
    cleanNemea.toUpperCase();
    return (cleanNemea);
  } else {
    return (inNmea183);
  }
}

