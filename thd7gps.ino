/*
  thd7gps - a small Arduino circuit to modify NMEA 183.3 to NMEA 183.2 sentencies
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
SoftwareSerial Radio(RADIORX, RADIOTX); // RX, TX
SoftwareSerial Gps(GPSRX, GPSTX); // RX, TX
char nmeaLine [85]; //max 82 for nmea standar
char cleanedNemeaLine [85];
int charIndex = 0;

bool receiving = true;
void setup() {

  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Radio.begin(RADIO_SPEED);
  Gps.begin(GPS_SPEED);

}

void loop() {
  Gps.listen();

  if (Gps.available() > 0) {

    inByte = Gps.read();

    if ( inByte != '\n') {
      nmeaLine[charIndex] = inByte;
      charIndex++;
    }
    else {
      charIndex = 0;
      receiving = false;
    }
  }
  if (!receiving) {
    if (convertNmea()) {
      Radio.println(cleanedNemeaLine);
      //Serial.println(cleanedNemeaLine);
    }
    memset(nmeaLine, 0, sizeof(nmeaLine)); // clean string
    receiving = true;
  }
}


bool convertNmea () {

  memset(cleanedNemeaLine, 0, sizeof(cleanedNemeaLine));
  int gprmcLatIndex = 4;
  int gprmcLonIndex = 6;
  int gpggaLatIndex = 2;
  int gpggaLonIndex = 4;

  if (strncmp(nmeaLine, "$GPRMC", 5) == 0 || strncmp(nmeaLine, "$GPGGA", 5) == 0 ) {

    int countComma = 0;
    int positionInit = 0;

    for (int index = 0;  index < 255; index++) {


      if (strncmp(nmeaLine, "$GPRMC", 5) == 0 && nmeaLine[index] == ',') {
        countComma += 1;
        if (countComma == gprmcLatIndex || countComma == gprmcLonIndex) {
          nmeaLine[index - 1] = '@';
        }

      }
      if (strncmp(nmeaLine, "$GPGGA", 5) == 0 && nmeaLine[index] == ',') {
        if (countComma == gpggaLatIndex || countComma == gpggaLonIndex) {
          nmeaLine[index - 1] = '@';
        }
        countComma += 1;
      }
    }


    int chk;
    // Remove the @ marker
    int cleanedNmeaIndex = 0;
    for (int index = 0;  index < 255; index++) {
      if (nmeaLine[index] != '@') {
        cleanedNemeaLine[cleanedNmeaIndex] = nmeaLine[index];
        if (cleanedNemeaLine[cleanedNmeaIndex] != '@'  && cleanedNemeaLine[cleanedNmeaIndex] != '$' && cleanedNemeaLine[cleanedNmeaIndex] != '*') {
          chk ^= cleanedNemeaLine[cleanedNmeaIndex];
        }
        cleanedNmeaIndex++;
      }
      if (nmeaLine[index] == '*') {
        break;
      }
    }
    char hexChk [2] ;
    sprintf(hexChk, "%0X", chk);
    strncat(cleanedNemeaLine, hexChk, 2);
    return true;
  }
  else {
    return false;
  }

}


