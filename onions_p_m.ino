#include <SoftwareSerial.h>
#include <stdlib.h>
#include <DHT.h>
#define DHT11_PIN 4
#define LM35_PIN A1
#define MQ135_PIN A0

DHT dht(DHT11_PIN, DHT11);
int gas_read = 0;
int t = 0;

String apiKey = "[2LWI103G8MFL2LJ2]";
SoftwareSerial ser(13,12); // RX, TX

void setup() {   
  pinMode(DHT11_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  Serial.begin(9600);
  // enable software serial
  ser.begin(9600);

  // reset ESP8266 WiFi connection AT+CIPMUX=1 AT+CWJAP
  ser.println("AT");
  delay(1000);
  ser.println("AT+GMR");
  delay(1000);
  ser.println("AT+CWMODE=3");
  delay(1000);
  ser.println("AT+RST");
  delay(5000);
  ser.println("AT+CIPMUX=1");
  delay(1000);
  //String cmd="AT+CWJAP="[Galaxy A20s0974]","[20BPS1156$]"";
  String cmd="AT+CWJAP=[Galaxy A20s0974]","[20BPS1156$]";
  ser.println(cmd);
  delay(1000);
  ser.println("AT+CIFSR");
  delay(1000);
 }

void loop(){
  delay(2000);
  gas_read = analogRead(MQ135_PIN);
  Serial.print("Ammonium: ");
  Serial.print(gas_read);
  
  int h = dht.readHumidity();
  Serial.print("\tHumidity: ");
  Serial.print(h);

  int temp_val;
  int temp;
  temp_val = analogRead(LM35_PIN); 
  temp = (temp_val * 500) / 1023;
  Serial.print("\tTemperature: ");
  Serial.print(temp);
  Serial.println(" C");
  // TCP connection AT+CIPSTART=4,"TCP","184.106.153.149",80 
  String cmd = "AT+CIPSTART=4","TCP","";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "","80";
  ser.println(cmd);
  Serial.println(cmd); 
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  // prepare GET string GET https://api.thingspeak.com/update?api_key=LHAG4NSIYJ5UWS6U&field1=0rnrn
  String getStr = "GET https://api.thingspeak.com/apps/thinghttp/send_request?api_key=2LWI103G8MFL2LJ2";
  getStr += apiKey;
  getStr +="&field1=";
  getStr +=String(temp);
  getStr +="&field2=";
  getStr +=String(h);
  getStr +="&field3=";
  getStr +=String(gas_read);
  getStr += "rnrn";
  // send data length
  cmd = "AT+CIPSEND=4,";
  cmd += String(getStr.length());
  ser.println(cmd);
  Serial.println(cmd);
  delay(1000);
  ser.print(getStr);
  Serial.println(getStr);
  delay(2000);
}
