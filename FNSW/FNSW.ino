
#include <eml_setop.h>
#include <peak.h>
#include <peak_data.h>
#include <peak_emxAPI.h>
#include <peak_emxutil.h>
#include <peak_initialize.h>
#include <peak_terminate.h>
#include <peak_types.h>
#include <rtGetInf.h>
#include <rtGetNaN.h>
#include <rtwtypes.h>
#include <rt_nonfinite.h>
#include <tmwtypes.h>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
#include <string.h>
#include <SD.h>

Adafruit_MPU6050 mpu;
File myFile;

// Neo pixel connection
#define NEOPIN      26
#define NUMPIXELS   1
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

// File creation variables
String filename;
int buttonhold = 0;
int filehold = 0;
int filenum = 80;
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

// maximum number of samples for the Fixed-size Non-overlapping Sliding Window (FNSW)
const int sampleSize = 500;

// MPU6050 sensor measurement variables
float AccX[sampleSize];
float AccY[sampleSize];
float AccZ[sampleSize];
float GyrX[sampleSize];
float GyrY[sampleSize];
float GyrZ[sampleSize];

// Variables for peak function (findpeaks - MATLAB)
// Along accelerometer Y-axis
double inputY[sampleSize];
double outputY[sampleSize];
double peaksY;

// Variables for peak function (findpeaks - MATLAB)
// Along accelerometer Z-axis
double inputZ[sampleSize];
double outputZ[sampleSize];
double peaksZ;

// Variables to count the nnumber of samples collected for FNSW window
int count = 0;
int range = 0;

// Variables to record the timestamp for identified peaks
unsigned long timeStamp[sampleSize];
unsigned long peaksY_timeStamp = 0;
unsigned long peaksZ_timeStamp = 0;
unsigned long sam_coll_time;
const unsigned long sam_time = 700000; // FNSW window size (ms)
 
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

	  // Adjusting the end time for FNSW based on the user button press action 	
      sam_coll_time = currentMicros + sam_time + 5000000;

     // Initializing TF card reader module
      if (!SD.begin(A5))
      {
        Serial.println("No SD Card Inserted");
        vibrate(50);
        while (1);
      }
	  
	  // File name	  
      filename = "/test";
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
	
	  // Reseting the FNSW window time	
      sam_coll_time = 0;
	  
	  // Reseting the values passed to the peak function
      if (count != 0)
      {
        for (int p = count + 1; p >= 0; p--)
        {
          inputY[p] = 0;
          inputZ[p] = 0;
        }
        count = 0;
      }

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

  // Data collection
  if (filehold == 20)
  {
	// Setting the neo pixel color - green 	  
    setPixel(0, 50, 0);

	// Data collection for the FNSW
    if (currentMicros <= sam_coll_time)
    {
      timeStamp[count] = currentMicros;

      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
	  
      AccX[count] = (double)a.acceleration.x; // Accelerometer X-axis value
      AccY[count] = (double)a.acceleration.y; // Accelerometer Y-axis value
      AccZ[count] = (double)a.acceleration.z; // Accelerometer Z-axis value
      GyrX[count] = (double)g.gyro.x;         // Gyroscope X-axis value 
      GyrY[count] = (double)g.gyro.y;         // Gyroscope Y-axis value
      GyrZ[count] = (double)g.gyro.z;		  // Gyroscope Z-axis value

	  // EWMA filter implementation => s_t = alpha*x_t + (1 - alpha)*s_(t-1) 	
      ewma_ax = (ewmaAlpha_ax * AccX[count]) + (1 - ewmaAlpha_ax) * ewma_ax;
      ewma_ay = (ewmaAlpha_ay * AccY[count]) + (1 - ewmaAlpha_ay) * ewma_ay;
      ewma_az = (ewmaAlpha_az * AccZ[count]) + (1 - ewmaAlpha_az) * ewma_az;
      ewma_gx = (ewmaAlpha_gx * GyrX[count]) + (1 - ewmaAlpha_gx) * ewma_gx;
      ewma_gy = (ewmaAlpha_gy * GyrY[count]) + (1 - ewmaAlpha_gy) * ewma_gy;
      ewma_gz = (ewmaAlpha_gz * GyrZ[count]) + (1 - ewmaAlpha_gz) * ewma_gz;

	  // peak function inputs	
      inputY[count] = ewma_ay;
      outputY[count] = 0;

      inputZ[count] = ewma_az;
      outputZ[count] = 0;

      peaksY = 0;
      peaksZ = 0;
      peaksY_timeStamp = 0;
      peaksZ_timeStamp = 0;

	  // Pressure sensor values
      sensorValue_left = analogRead(analogInPin_left);
      sensorValue_right = analogRead(analogInPin_right);
	  
      // Data writing into the file	
      filesave();
      count = count + 1;
    } else // Identifying peaks from the data collected in FNSW
    {
      int Size = count;
      
	  // Initialization of peak function input arguments
	  // Along accelerometer Y-axis	
	  emxArray_real_T *func_inY = NULL;
      emxArray_real_T *func_outY = NULL;

	  // Initialization of peak function input arguments
	  // Along accelerometer Z-axis
      emxArray_real_T *func_inZ = NULL;
      emxArray_real_T *func_outZ = NULL;

	  // Variable convertion methodology for the peak function build from MATLAB code builder
	  // Along accelerometer Y-axis	  
      func_inY = emxCreateWrapper_real_T(inputY, 1, Size); // 1 - row, Size - colums
      func_outY = emxCreateWrapper_real_T(outputY, 1, Size);

	  // Variable convertion methodology for the peak function build from MATLAB code builder
	  // Along accelerometer Z-axis
      func_inZ = emxCreateWrapper_real_T(inputZ, 1, Size);
      func_outZ = emxCreateWrapper_real_T(outputZ, 1, Size);

	  // peak function call	
      peak(func_inY, func_outY);
      peak(func_inZ, func_outZ);

	  // Condition if no peaks are detected by the peak function along Y-axis & Z-axis	
      if (func_outY->size[1] == 0 && func_outZ->size[1] == 0)
      {
        count = 0;
        timeStamp[count] = currentMicros;
        peaksY = 0;
        peaksZ = 0;
        peaksY_timeStamp = 0;
        peaksZ_timeStamp = 0;

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
		
        AccX[count] = (double)a.acceleration.x; // Accelerometer X-axis value
        AccY[count] = (double)a.acceleration.y; // Accelerometer Y-axis value
        AccZ[count] = (double)a.acceleration.z; // Accelerometer Z-axis value
        GyrX[count] = (double)g.gyro.x;         // Gyroscope X-axis value 
        GyrY[count] = (double)g.gyro.y;         // Gyroscope Y-axis value
        GyrZ[count] = (double)g.gyro.z;		    // Gyroscope Z-axis value

	    // EWMA filter implementation => s_t = alpha*x_t + (1 - alpha)*s_(t-1) 
        ewma_ax = (ewmaAlpha_ax * AccX[count]) + (1 - ewmaAlpha_ax) * ewma_ax;
        ewma_ay = (ewmaAlpha_ay * AccY[count]) + (1 - ewmaAlpha_ay) * ewma_ay;
        ewma_az = (ewmaAlpha_az * AccZ[count]) + (1 - ewmaAlpha_az) * ewma_az;
        ewma_gx = (ewmaAlpha_gx * GyrX[count]) + (1 - ewmaAlpha_gx) * ewma_gx;
        ewma_gy = (ewmaAlpha_gy * GyrY[count]) + (1 - ewmaAlpha_gy) * ewma_gy;
        ewma_gz = (ewmaAlpha_gz * GyrZ[count]) + (1 - ewmaAlpha_gz) * ewma_gz;

	    // peak function inputs	
        inputY[count] = ewma_ay;
        outputY[count] = 0;

        inputZ[count] = ewma_az;
        outputZ[count] = 0;

	    // Pressure sensor values
        sensorValue_left = analogRead(analogInPin_left);
        sensorValue_right = analogRead(analogInPin_right);

        // Data writing into the file	
        filesave();

        count = count + 1;
      } else // if atleast one peak is detected by the peak function
      {
		  
		// Logic to identify the maximum number of peaks found in Y-axis & Z-axis
		// This is required for writing the peaks into the SD card	
        if (func_outY->size[1] < func_outZ->size[1])
        {
          range = (int)func_outZ->size[1];

        } else if (func_outY->size[1] > func_outZ->size[1])
        {
          range = (int)func_outY->size[1];

        } else if (func_outY->size[1] == func_outZ->size[1])
        {
          range = (int)func_outZ->size[1];
        }

        count = 0;
        timeStamp[count] = currentMicros;

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
		
        AccX[count] = (double)a.acceleration.x; // Accelerometer X-axis value
        AccY[count] = (double)a.acceleration.y; // Accelerometer Y-axis value
        AccZ[count] = (double)a.acceleration.z; // Accelerometer Z-axis value
        GyrX[count] = (double)g.gyro.x;         // Gyroscope X-axis value 
        GyrY[count] = (double)g.gyro.y;         // Gyroscope Y-axis value
        GyrZ[count] = (double)g.gyro.z;		    // Gyroscope Z-axis value

	    // EWMA filter implementation => s_t = alpha*x_t + (1 - alpha)*s_(t-1) 
        ewma_ax = (ewmaAlpha_ax * AccX[count]) + (1 - ewmaAlpha_ax) * ewma_ax;
        ewma_ay = (ewmaAlpha_ay * AccY[count]) + (1 - ewmaAlpha_ay) * ewma_ay;
        ewma_az = (ewmaAlpha_az * AccZ[count]) + (1 - ewmaAlpha_az) * ewma_az;
        ewma_gx = (ewmaAlpha_gx * GyrX[count]) + (1 - ewmaAlpha_gx) * ewma_gx;
        ewma_gy = (ewmaAlpha_gy * GyrY[count]) + (1 - ewmaAlpha_gy) * ewma_gy;
        ewma_gz = (ewmaAlpha_gz * GyrZ[count]) + (1 - ewmaAlpha_gz) * ewma_gz;

	    // peak function inputs	
        inputY[count] = ewma_ay;
        outputY[count] = 0;

        inputZ[count] = ewma_az;
        outputZ[count] = 0;

	    // Pressure sensor values
        sensorValue_left = analogRead(analogInPin_left);
        sensorValue_right = analogRead(analogInPin_right);

        // Data writing into the file along with peaks and peaks timestamps 
        for (int i = 0; i < range; ++i)
        {
		  // Logic to determine the peaks timestamps along Y-axis	
          if (i < (int)func_outY->size[1])
          {
            int j;
            peaksY = double(func_outY->data[i]);

            for (j = 0; j < Size; j++)
            {
              if (inputY[j] == peaksY)
              {
                break;
              }
            }
            peaksY_timeStamp = timeStamp[j];

          } else
          {
            peaksY = 0;
            peaksY_timeStamp = 0;
          }

		  // Logic to determine the peaks timestamps along Z-axis
          if (i < (int)func_outZ->size[1])
          {
            int j;
            peaksZ = double(func_outZ->data[i]);

            for (j = 0; j < Size; j++)
            {
              if (inputZ[j] == peaksZ)
              {
                break;
              }
            }
            peaksZ_timeStamp = timeStamp[j];

          } else
          {
            peaksZ = 0;
            peaksZ_timeStamp = 0;
          }

          filesave();
        }

        count = count + 1;
		
		// Updating the FNSW for next the window
        sam_coll_time = currentMicros + sam_time;
      }
	  
	  // Destroying the peak function input argument variables
      emxDestroyArray_real_T(func_inY);
      emxDestroyArray_real_T(func_outY);
      emxDestroyArray_real_T(func_inZ);
      emxDestroyArray_real_T(func_outZ);
    }
  }
}

// Function to write data to SD card
void filesave()
{
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
    myFile.print(count);
    myFile.print(", ");
    myFile.print(sam_coll_time);
    myFile.print(", ");
    myFile.print(timeStamp[count]);
    myFile.print(", ");
    myFile.print(AccX[count]);
    myFile.print(", ");
    myFile.print(AccY[count]);
    myFile.print(", ");
    myFile.print(AccZ[count]);
    myFile.print(", ");
    myFile.print(GyrX[count]);
    myFile.print(", ");
    myFile.print(GyrY[count]);
    myFile.print(", ");
    myFile.print(GyrZ[count]);
    myFile.print(", ");
    myFile.print(sensorValue_left);
    myFile.print(", ");
    myFile.print(sensorValue_right);
    myFile.print(", ");
    myFile.print(peaksY);
    myFile.print(", ");
    myFile.print(peaksY_timeStamp);
    myFile.print(", ");
    myFile.print(peaksZ);
    myFile.print(", ");
    myFile.print(peaksZ_timeStamp);
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
    vibrate(50);
  }
}
