#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>    //memasukan library Software Serial
#include <DFPlayer_Mini_Mp3.h> //memasukan library DFPlayermini

#include <MD_Parola.h>
#include <MD_MAX72xx.h>

const uint16_t WAIT_TIME = 1000;

#define MAX_DEVICES 1
#define CLK_PIN   52
#define DATA_PIN  51
#define CS_PIN    53

MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
char server[]="192.168.137.1"; //Domain atau Address WEBSERVER
String host="Host: 192.168.137.1"; //Domain atau Address WEBSERVER
//IPAddress ip(192,168,137,2);//IP Address Ethernet Shield
IPAddress  localBoardIP;
EthernetClient client;
//------------------------------------

int GateCOM=20;
String kode="";
boolean state=false;
  void ToBoolean(String value)
    {
        if(value=="true" ){
            state=true;
           
          }else{  
           state=false;  
          }
    }
 void Cetak(String Display,String Value){
  if(Display == "S"){
  Serial.println(Value);
    }else{
      //print with Other Display
      }
  }

void GetFromDb(String value){
   
     value.trim();    
     String valueGet="data="+value;
     Cetak("S","Log Value");
     Cetak("S",valueGet);
     
    if (client.connect(server, 80)) {    
        Cetak("S","GET Connect");
        client.println("POST /Entry-Gate-Bank-Nagari/GATE_TIKET/index.php HTTP/1.1");
        client.println(host);
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Connection: close");
        client.print("Content-Length: ");
        client.println(valueGet.length());
        client.println();
        client.print(valueGet);
        client.println();

        client.println();
         delay(100);
                String data= client.readString();
                 Serial.println("Karakter yang diterima");
                 Serial.println("------------------------------");
                 Serial.println(data);
                 Serial.println("-----------------------------");
//                Serial.println(data);
                                int indexBatasAwal=data.indexOf("_start_");
                Serial.println("Mulai Index Karakter String seharusnya: ");
                Serial.println(indexBatasAwal);
                Serial.println("Mulai Karakter String seharusnya: ");
                String json=(String)data.substring(indexBatasAwal+8);
                Serial.println(json);
                ToBoolean(json);

    }else{
     Cetak("S","Connections Filed ..!!");
     delay(100);
    }
    client.stop();
    }

    void insertIntoDb(String value){
   
     value.trim();    
     String valueGet="data="+value;
     Cetak("S","Log Value");
     Cetak("S",valueGet);
     
    if (client.connect(server, 80)) {    
        Cetak("S","GET Connect");
        client.println("POST /Entry-Gate-Bank-Nagari/GATE_TIKET/simpan.php HTTP/1.1");
        client.println(host);
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Connection: close");
        client.print("Content-Length: ");
        client.println(valueGet.length());
        client.println();
        client.print(valueGet);
        client.println();

        client.println();
         delay(100);
                String data= client.readString();
                 Serial.println("Karakter yang diterima");
                 Serial.println("------------------------------");
                 Serial.println(data);
                 Serial.println("-----------------------------");
//                Serial.println(data);
                                int indexBatasAwal=data.indexOf("_start_");
                Serial.println("Mulai Index Karakter String seharusnya: ");
                Serial.println(indexBatasAwal);
                Serial.println("Mulai Karakter String seharusnya: ");
                String json=(String)data.substring(indexBatasAwal+8);
                Serial.println(json);
               if(json=="0001"){
                //dewasa
                P.print("D");
                digitalWrite(14, LOW);
                mp3_play (1); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3
                delay (1200); //jeda 10 detik
               }else if(json=="0002"){
                //anak-anak
                P.print("A");
                digitalWrite(15, LOW);
                mp3_play (2); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3
                delay (1200); //jeda 10 detik
               }else{
                //putar musik turis
                P.print("T");
                mp3_play (3); //memainkan lagu 1 pada folder mp3 yang sudah direname dengan nama 0001.mp3
                delay (5200); //jeda 10 detik
               }

    }else{
     Cetak("S","Connections Filed ..!!");
     delay(100);
    }
    client.stop();
    }





void setup() {
  Serial.begin(9600); 
  Serial2.begin(9600); 
  Serial1.begin(9600);
    P.begin();
  Serial1.setTimeout(100);
  mp3_set_serial (Serial2); 
  delay(5); 
  mp3_set_volume (100);

//  ----------------Ethernet Intialiting
  Serial.println("Connecting..");
  Ethernet.begin(mac);
  Serial.println("Conncted on Local Network with your IP:");
  localBoardIP=Ethernet.localIP();
  Serial.println(localBoardIP);
  Serial.println("Start");
//  -------------END
pinMode(GateCOM, OUTPUT);
digitalWrite(GateCOM, HIGH);

pinMode(14, OUTPUT);
digitalWrite(14, HIGH);

pinMode(15, OUTPUT);
digitalWrite(15, HIGH);

// LED Matrix


   
}

void loop() {
  P.print("X");
  delay(400);
  digitalWrite(14, HIGH);
  digitalWrite(15, HIGH);
  kode="";
  while (Serial1.available() > 0) {
    kode = Serial1.readString();
    Serial.println(kode);
     if(kode.length()>=64){
      P.print("?");
      Serial.println(kode);
     GetFromDb(kode);
    break;
      }
  }
 
  if(state){
   
      digitalWrite(GateCOM, LOW); // sets the digital pin 13 on
  delay(1000);            // waits for a second
  digitalWrite(GateCOM, HIGH);  // sets the digital pin 13 off
  state=false;
  delay(1000); 
   insertIntoDb(kode);
   
  }
}
