
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <string.h>
#include <SPI.h>
#include <SD.h>

Adafruit_MPU6050 mpu;
File myFile;

// Neo pixel connection
#define NEOPIN      26
#define NUMPIXELS   1
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

// Frequency setting variables
unsigned long timeloop;

// MPU6050 sensor measurement variables
float AccX;
float AccY;
float AccZ;
float GyrX;
float GyrY;
float GyrZ;

// File creation variables
String filename;
int buttonhold = 0;
int filehold = 0;
int filenum = 10;
char string[20];

// Button connection
const int PushButton = 14;

// Right Foot Pressure Sensor!!!!!!
const int analogInPin_left = A1; //black and red wire
int sensorValue_left = 0;

// Left Foot Pressure Sensor!!!!!!
const int analogInPin_right = A2; //green and red wire
int sensorValue_right = 0;

float ewmaAlpha_ax = 0.01;  // the EWMA alpha value (α) for accelerometer X-axis
double ewma_ax = 0;

float ewmaAlpha_ay = 0.01;  // the EWMA alpha value (α) for accelerometer Y-axis
double ewma_ay = 0;

float ewmaAlpha_az = 0.01;  // the EWMA alpha value (α) for accelerometer Z-axis
double ewma_az = 0;

float ewmaAlpha_gx = 0.01;  // the EWMA alpha value (α) for gyroscope X-axis
double ewma_gx = 0;

float ewmaAlpha_gy = 0.01;  // the EWMA alpha value (α) for gyroscope Y-axis
double ewma_gy = 0;

float ewmaAlpha_gz = 0.01;  // the EWMA alpha value (α) for gyroscope Z-axis
double ewma_gz = 0;

void setup()
{

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  if (!SD.begin(A5)) {
    Serial.println("No SD Card Inserted");
    vibrate(50);
    while (1);
  }
  	
  init_mpu();

  init_neopixel();

  init_vib();

  pinMode(PushButton, INPUT);
}

void loop()
{

  unsigned long currentMicros = micros();
  
  // Button press read
  int Push_button_state = digitalRead(PushButton);
  int button = (Push_button_state == HIGH ? 0 : 100);
  
  // Button pressed action
  if ( button == 100 )
  {
	// File creation and starting data collection - yellow light
    if (buttonhold == 0)
    {
      filehold = 20;

      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      ewma_ax = a.acceleration.x; // Accelerometer X-axis value
      ewma_ay = a.acceleration.y; // Accelerometer Y-axis value
      ewma_az = a.acceleration.z; // Accelerometer Z-axis value
      ewma_gx = g.gyro.x;         // Gyroscope X-axis value
      ewma_gy = g.gyro.y;         // Gyroscope Y-axis value 
      ewma_gz = g.gyro.z;         // Gyroscope Z-axis value 

      // Initializing TF card reader module
      if (!SD.begin(A5))
      {
        Serial.println("No SD Card Inserted");
        vibrate(50);
        while (1);
      }
	  
	  // File name
      filename = "/Run";
      itoa(filenum, string, 10);
      filename = filename + string + ".txt";
	  
	  // Initializing write mode for the file
      myFile = SD.open("filename", FILE_WRITE);
	  
	  // Incrementing filenumber for next run
      filenum = filenum + 1;
	  
	  // Setting the neo pixel color - yellow
      setPixel(50, 50, 0);
    }

    vibrate(100);
	
	// File closing - red light
    if (buttonhold == 10)
    {
      filehold = 0;

      // Reseting the initial values of EWMA filter for next run
      ewma_ax = 0; 
      ewma_ay = 0; 
      ewma_az = 0; 
      ewma_gx = 0;
      ewma_gy = 0;
      ewma_gz = 0;

	  // Setting the neo pixel color - red
      setPixel(50, 0, 0);
	  
      vibrate(100);
      delay(1000);
      vibrate(0);
      delay(1000);
      vibrate(100);
    }
	delay(5000);
  } else {
    setPixel(0, 0, 0);
    vibrate(0);
  }

  // Logic to control file creation for each run based on button press action
  if (filehold == 20)
  {
    buttonhold = 10;
  }
  if (filehold == 0)
  {
    buttonhold = 0;
  }

  // Data collection at 52 Hz
  if (filehold == 20)
  {
	// Frequency adjustment to 52 HZ  
    if (micros() > timeloop + 19200)
    {
      timeloop = micros();
	  
	  // Setting the neo pixel color - green 
      setPixel(0, 50, 0);

      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      AccX = a.acceleration.x; // Accelerometer X-axis value
      AccY = a.acceleration.y; // Accelerometer Y-axis value
      AccZ = a.acceleration.z; // Accelerometer Z-axis value
      GyrX = g.gyro.x;         // Gyroscope X-axis value
      GyrY = g.gyro.y;         // Gyroscope Y-axis value
      GyrZ = g.gyro.z;		   // Gyroscope Z-axis value

	  // EWMA filter implementation => s_t = alpha*x_t + (1 - alpha)*s_(t-1) 	
      ewma_ax = (ewmaAlpha_ax * AccX) + (1 - ewmaAlpha_ax) * ewma_ax;
      ewma_ay = (ewmaAlpha_ay * AccY) + (1 - ewmaAlpha_ay) * ewma_ay;
      ewma_az = (ewmaAlpha_az * AccZ) + (1 - ewmaAlpha_az) * ewma_az;
      ewma_gx = (ewmaAlpha_gx * GyrX) + (1 - ewmaAlpha_gx) * ewma_gx;
      ewma_gy = (ewmaAlpha_gy * GyrY) + (1 - ewmaAlpha_gy) * ewma_gy;
      ewma_gz = (ewmaAlpha_gz * GyrZ) + (1 - ewmaAlpha_gz) * ewma_gz;

	  // Pressure sensor values
      sensorValue_left = analogRead(analogInPin_left);
      sensorValue_right = analogRead(analogInPin_right);

      // Initializing TF card reader module
      if (!SD.begin(A5)) {
        Serial.println("initialization failed!");
        while (1);
      }
      
	  // Opening the file to record the data
      myFile = SD.open(filename, FILE_APPEND);

      // Data writing into the file	
      if (myFile)
      {
        vibrate(0);
        myFile.print(currentMicros);
        myFile.print(", ");
        myFile.print(AccX);
        myFile.print(", ");
        myFile.print(AccY);
        myFile.print(", ");
        myFile.print(AccZ);
        myFile.print(", ");
        myFile.print(GyrX);
        myFile.print(", ");
        myFile.print(GyrY);
        myFile.print(", ");
        myFile.print(GyrZ);
        myFile.print(", ");
        myFile.print(sensorValue_left);
        myFile.print(", ");
        myFile.print(sensorValue_right);
        myFile.print(", ");
        myFile.print(ewma_ax);
        myFile.print(", ");
        myFile.print(ewma_ay);
        myFile.print(", ");
        myFile.print(ewma_az);
        myFile.print(", ");
        myFile.print(ewma_gx);
        myFile.print(", ");
        myFile.print(ewma_gy);
        myFile.print(", ");
        myFile.print(ewma_gz);
        myFile.print(",\n");
        myFile.close();
      } else {
        Serial.println("error opening test.txt");
      }
    }
  }
}
