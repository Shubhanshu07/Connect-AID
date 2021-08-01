//Library
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <WiFiClient.h>
#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(13, 15); // The serial connection to the GPS device

float latitude , longitude;
String lat_str , lng_str;
int flag = 0;
int Stop = 0;

Adafruit_MPU6050 mpu;
float ax, ay, az, accelerationX, accelerationY, accelerationZ, roll, pitch, yaw;
#define CONVERSIONG 3.9 
int crash = 0;

//SSID
const char* ssid     = "ACTFIBERNET";
const char* password = "act12345";
//Host
const char* host = "http://192.168.0.200:5000"; 
//const char* host = "http://63fed4e00cbf.ngrok.io"; 

 
String url = "/addrec";

void setup() {

  // Serial
  Serial.begin(9600);
  while (!Serial)
    delay(10);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
     
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

 /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  ax = a.acceleration.x;
  ay = a.acceleration.y;
  az = a.acceleration.z;

  delay(500);
accelerationX = (int16_t)(ax * CONVERSIONG);
accelerationY = (int16_t)(ay * CONVERSIONG);
accelerationZ = (int16_t)(az * CONVERSIONG);
pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/M_PI;
roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;
yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;

Serial.print(" Crash : ");
Serial.print(crash);

Serial.print("\n");

if (abs(pitch) > 60)
{crash = 1;
}
else if ((abs(ay) - abs(ax) ) > 1)
{crash = 1;
}
else if(abs(ax) >= 6)
{crash = 1;
}
  
if (crash == 1){  
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
} 
//Serial.print("connecting to "); 
//Serial.println(host); // Use WiFiClient class to create TCP connections 
WiFiClient client; 

if(flag == 1 && crash == 1 && Stop == 0)
{
Serial.print("Requesting URL: "); 
Serial.println(url); //Post Data 
String postData = "id=HR1045@connectaide&latitude="+lat_str+"&longitude="+lng_str+"&severity=5&relative=9907031008"; 
Serial.println(postData);
Serial.print("\nY :");
Serial.print(yaw);
Serial.print(" P :");
Serial.print(pitch);
Serial.print(" R :");
Serial.print(roll);
Serial.print(" Ax :");
Serial.print(ax);
Serial.print(" Ay :");
Serial.print(ay);
Serial.print(" Az :");
Serial.print(az);
Serial.print("\n"); 

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
crash = 0;
Stop = 1;
}
} 
