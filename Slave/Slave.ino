//Slave script for SPPB Balance Test 

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SoftwareSerial.h"

SoftwareSerial WiFlySerial(8,7); //RX  TX       !!!!!!!!!!!!!!!!!!!!

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

int i = 0;
int ax1,ay1;
int ax2,ay2,mag = 0;
int threshold = 8000;

String TheSignal = "movement";                          //!!!!!!!!!!!!!!!!!!!

boolean flag_check = false;
boolean first_time = true;


void setup(void)
{
  Wire.begin();
  Serial.begin(9600);
  WiFlySerial.begin(9600);
  
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

//Functions to send data through WIFI
void WiFiSend(String data_send)
{
  WiFlySerial.print(data_send);
  WiFlySerial.println("\n");
}

void WiFiSend(unsigned int data_send)
{ 
  WiFlySerial.print(data_send);
  WiFlySerial.println("\n");
}


//Function to calculate magnitude of movement and check
//for passing the threshold
void getHeadingDegrees() {
  if (first_time == true)
  {
   ax1 = ax;
   ay1 = ay; 
   first_time = false;
  }
  
  ax2 = ax-ax1;
  ay2 = ay-ay1;
  mag = abs(ax2)+abs(ay2);
  Serial.println(mag);
  if (mag >= threshold)
  {
   flag_check = true; 
  }
  ax1 = ax;
  ay1 = ay;
}


void loop(void)
{

 
  //Motion Sensor. Checks for foot movement
 accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
 getHeadingDegrees();


  //When movement, signals to master 
  if (flag_check == true)
  {
    Serial.println("Movement Detected");
    WiFiSend(TheSignal);                                //!!!!!!!!!!!!!!!!!
    flag_check = false;
  }
 
}

