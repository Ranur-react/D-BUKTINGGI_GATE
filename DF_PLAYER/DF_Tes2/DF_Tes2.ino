#include <SoftwareSerial.h>    //memasukan library Software Serial
#include <DFPlayer_Mini_Mp3.h> //memasukan library DFPlayermini

//SoftwareSerial mySerial(3, 2); //pin RX dan TX

void setup () { 
  Serial2.begin (9600); //baud komunikasi pada 9600
  mp3_set_serial (Serial2); 
  delay(5); 
  mp3_set_volume (100);
}

void loop () {   
mp3_play (1); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3
delay (2000); //jeda 10 detik
mp3_play (2); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3;.,'
delay (2000); //jeda 10 detik  
mp3_play (3); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3;.,'
delay (5000); //jeda 10 detik  
}
