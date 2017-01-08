# thd7gps
The TH-D7 uses two NMEA sentences out of the many sent by the GPS unit every second: the $GPGGA and the $GPRMC.  
Early firmware version did not recognize the commas separating the various fields inside these sentences, using instead a fixed-length method that could lead to wrong data interpretation.
Let's look more in detail to the following examples of NMEA sentences as sent, respectively, by a non-compatible GPS and a compatible one:

NMEA 183.3 GPS sends:

$GPRMC,192944,V,3733.8310,N,01504.5017,E,20.0,270.8,081205,2.0,E,S*23
$GPGGA,192958,3733.8070,N,01504.4222,E,8,09,2.0,335.4,M,38.3,M,,*4A



NMEA 183.2 GPS sends:

$GPRMC,192944,V,3733.831,N,01504.501,E,20.0,270.8,081205,2.0,E,S*23
$GPGGA,192958,3733.807,N,01504.422,E,8,09,2.0,335.4,M,38.3,M,,*4A

The 4 decimal digits in the Lat and Long fields instead of the 3 expected by the TH-D7 are the source of the incompatibility, so why not to snip the extra digit before it reaches the TH-D7? Our "potator" (ital-english term for "snipper")  project is based on this idea, simple but needing a minimum of hardware able to:

receive the NMEA strings



identify the $GPGGA and $GPRMC sentences


remove the least significant decimal digits exceeding the 3 expected by the radio


recalculate the checksum


Reference http://www.qsl.net/n6qab/n6qabgps1.htm

(c) 2017 Paolo Mattiolo In3aqk

