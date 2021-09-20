#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
char server[]="192.168.137.1"; //Domain atau Address WEBSERVER
String host="Host: 192.168.137.1"; //Domain atau Address WEBSERVER
//IPAddress ip(192,168,137,2);//IP Address Ethernet Shield
IPAddress  localBoardIP;
EthernetClient client;
//------------------------------------
// defines variables Sensor Ultra Sonic---------
const int trigPin = 17;
const int echoPin = 16;
long duration;
int distance;
//--------------------------------------------
int GateCOM=20;
int GateCOM2=21;
//--state data QR--------------------
String kode="";
//----end
//---state gerbang------
boolean state=false;
//end------------------
//-----kondisi---dewasa=true--anak-anak=false
String adult="false";
//----end
void distance_sensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
    Serial.print("Disatance: ");
    Serial.println(distance);
}




  void ToBoolean(String value)
    {
        if(value =="true" ){
          Serial.println("2. Kondisi State TRUE");
          state=true;
          }else{
            state=false;  
            Serial.println("2. Kondisi State FALSE");
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
     String valuePOST1="data="+value;
     String valuePOST2="adult="+adult;
     String data=valuePOST1+"&"+valuePOST2;
     Cetak("S","Log Value");
     Cetak("S",valuePOST1);
     Cetak("S","DEWASa Value");
     Cetak("S",valuePOST2);
     
    if (client.connect(server, 80)) {    
        Cetak("S","POST Connect");
        client.println("POST /Entry-Gate-Bank-Nagari/GATE_TIKET/tes_Post.php HTTP/1.1");
        client.println(host);
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.println("Connection: close");
        client.print("Content-Length: ");
        client.println(data.length());
        client.println();
        client.print(data);
        client.println();

        client.println();
//         delay(100);
                String data= client.readString();
//                Serial.println(data);
                                int indexBatasAwal=data.indexOf("_start_");
                Serial.println("Mulai Index Karakter String seharusnya: ");
                Serial.println(indexBatasAwal);
                Serial.println("Mulai Karakter String seharusnya: ");
                String json=(String)data.substring(indexBatasAwal+8);
                        json.trim();
                Serial.println(json);
                Serial.println("_1. Kondisi State From SERVER");
                ToBoolean(json);
    }else{
     Cetak("S","Connections Filed ..!!");
     delay(100);
    }
    client.stop();
    }




void inisialisasNet(){
   if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }}
    Serial.print("Connected");
      Serial.println("Conncted on Local Network with your IP:");
  localBoardIP=Ethernet.localIP();
  Serial.println(localBoardIP);
}
void pindaiUkurantubuh(){
    int loopcount=0;long starttime = millis();long endtime = starttime;
    while((endtime - starttime) <= 500 ) // do this loop for up to 1000mS
    {
      //command
      distance_sensor();
      if (distance<80){
        delay(100);
        if (distance<80){
            adult="true";
            break;  
          }
        }else{
            adult="false";
            }
      //end-page-commadn
      loopcount = loopcount+1;endtime = millis();
      Serial.println("Test Loop Timer");
    }
    Serial.println("Scan Finised :");
    Serial.print (loopcount,DEC);
    Serial.print(" s");
    GetFromDb(kode);
  }
void setup() {
  Serial.begin(9600); 


//  ----------------Ethernet Intialiting
  Serial.println("Connecting..");
inisialisasNet();
  Serial1.begin(9600);
    Serial1.setTimeout(100);
    Serial.println("Start");
//  -------------END
pinMode(GateCOM, OUTPUT);
digitalWrite(GateCOM2, LOW);
digitalWrite(GateCOM, HIGH);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  kode="";
  while (Serial1.available() > 0) {
    kode = Serial1.readString();
    Serial.print("Jumlah karakter Chipper Teks");
    Serial.print(kode.length());
    if(kode.length()>=65){
      Serial.println(kode);
pindaiUkurantubuh();
    break;
      }
    
    
  }
  if(state){
    Serial.println("3. Kondisi OUTPUT HIGH");
      digitalWrite(GateCOM2, HIGH); 
      digitalWrite(GateCOM, LOW);
  delay(100);            // waits for a second
  digitalWrite(GateCOM2, LOW);  // sets the digital pin 13 off
  digitalWrite(GateCOM, HIGH);
  state=false;
  delay(100); 
  }
}
