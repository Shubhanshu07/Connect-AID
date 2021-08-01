//Library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <WiFiClient.h>
#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(13, 15); // The serial connection to the GPS device

float latitude , longitude;
String lat_str , lng_str;
int flag = 0;
int flag2 = 1;
//SSID
const char* ssid     = "ACTFIBERNET";
const char* password = "act12345";
//Host
//const char* host = "http://192.168.0.200:5000"; 
const char* host = "http://63fed4e00cbf.ngrok.io"; 

 
String url = "/addrec";

void setup() {

  // Serial
  Serial.begin(9600);
  ss.begin(9600);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

 // WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{ 

while (ss.available() > 0) //while data is available
    if (gps.encode(ss.read())) //read gps data
    { //Serial.print("reading GPS data\n");
      if (gps.location.isValid()) //check whether gps location is valid
      { //Serial.print("inside GPS location\n");
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude location is stored in a string
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude location is stored in a string
        Serial.print(lat_str);
        Serial.print("$$");
        Serial.print(lng_str);
        Serial.print("\n");
        flag=1;
      }
    }
 
//Serial.print("connecting to "); 
//Serial.println(host); // Use WiFiClient class to create TCP connections 
WiFiClient client; 

if(flag == 1 && flag2 == 1)
{
Serial.print("Requesting URL: "); 
Serial.println(url); //Post Data 
String postData = "id=HR1045@connectaide&latitude="+lat_str+"&longitude="+lng_str+"&severity=5&relative=9907031008"; 
Serial.println(postData); 
String address = host + url; 
Serial.println(address);
HTTPClient http; 
http.begin(client, address); //obsolete API
http.addHeader("Content-Type", "application/x-www-form-urlencoded");//"text/plain"); // 

auto httpCode = http.POST(postData); 
Serial.println(httpCode); //Print HTTP return code 
String payload = http.getString(); 
Serial.println(payload); //Print request response payload 
http.end(); //Close connection Serial.println(); 
Serial.println("closing connection"); 
flag = 0;
flag2 = 0;
}
} 
