#include <SPI.h>          //******** SPI Communication with the Card
#include <SD.h>           //******** Include SD library fror reading and writing to SD Card
#include <Adafruit_GPS.h> //******** Include Library for GPS functions

#define GPSSerial Serial1 //******** Communicate with GPS on TX1 and RX1 pins
Adafruit_GPS GPS(&GPSSerial);//***** Connect to the GPS on the Hardware Port

//Define SPI pins********************************
#define CS 10    // SPI chip select pin
#define MOSI 11  // SPI master out, slave in pin
#define MISO 12  // SPI master in, slave out pin
#define CLK 13   // SPI clock pin

File myFile;     // File object
char fileName[21] = "GDAT0.csv"; //Name of file to be created on the SD Card
int fileNumber=0, writeCount; 
String tempstring, Fdata;

int Hour, Min, Seconds, Day, Month, Year, Fix,FixQ, NumSats;
float Lat, Longi, Alti, Temp, Press;

double m =-0.25;
double yintercept = 90;

String file_header1= "Capstone experiment at Grambling State university and the labels for the columns are as follows:";
String file_header2= "Time, Date, Fix, FixQuality, #sat, Longitude, Latitude, Altitude, Temperature, Pressure";


/* Other Gobal Variables*********************************************/
boolean usingInterrupt = false;     //Falg for if GPS interrupt should be enabled
void useInterrupt(boolean);;        //Prototype of function to enable/ disable interrupt
void GPS_config();;
 /************************************/


//

//


void setup() {

 Serial.begin(115200);                        //begin UART communiation
 Serial1.begin(9600);                         //begin UART communiation for GPS
 GPS_config();                                //Configure GPS Settings
 
 
 Serial.print("Initializing SD card...");
 if (!SD.begin(CS, MOSI, MISO, CLK)){                //initialize communication with the SD Card    
    Serial.print("Card failed, or not present : ");   //if initialization failed
    while (1) ;                                       //wait here until reset. (Forever loop)
 }
 Serial.println("card initialized.");

//




//

                                                      // Open up the file
 myFile = SD.open(fileName, FILE_WRITE);
 if (! myFile) {
     Serial.println("There was an error opening " +String(fileName));  //pause if file not opened
     while (1) ;  // Wait forever since we cant write data
 }

Serial.println("There following file was oppened: " +String(fileName));  //continue to loop if file opened
  myFile.println(file_header1);
  myFile.println(file_header2); //add header to file
  
useInterrupt(true);                          //use interrupt to read GPS output
}

/****************************Interupt service routine for GPS****/
SIGNAL(TIMER0_COMPA_vect){
//This ISR reads a character form the GPS
char c = GPS.read();           // Read a character from the GPS  
}
//


//
void loop() {

//
//Serial.print("   Temp(ADC)= ");
//Serial.println(analogRead(A0));
//Serial.print("    Temperature=  ");
//Serial.println((analogRead(A0))*m + yintercept);
//

//print NMEA if received

if(writeCount >60){ newFile();}
if(GPS.newNMEAreceived())
 {
  delay(1000);
  if(GPS.parse(GPS.lastNMEA())){
    Serial.println(GPS.lastNMEA());
    Serial.println("Parse was sucessfull  ");
    Day=GPS.day;

Month=GPS.month;
    Year=GPS.year;
    Hour=GPS.hour;
    Seconds=GPS.seconds;
    Min=GPS.minute;
    Fix=GPS.fix;
    FixQ=GPS.fixquality;
    NumSats=GPS.satellites;
    Lat=GPS.latitudeDegrees;
    Longi=GPS.longitudeDegrees;
    Alti=GPS.altitude;
    //Temp=getTemperature(); //get temperature data from function
    Temp=(analogRead(A0))*m + yintercept;

    Press=getPressure(); //get pressure data from function


/****************************Save variable to string*********************************/  
        //Fdata = String(Hour)+":"+String(Min)+":"+String(Seconds)+","+String(Day)+"/"+String(Month)+"/"+String(Year)+
        //        String(Fix)+","+String(FixQ)+","+String(NumSats)+","+String(Lat)+","+String(Longi)+","+String(Alti)+","+String(Temp)+","+String(Press);

        Fdata = String(Hour)+":"+String(Min)+":"+String(Seconds)+","+String(Day)+"/"+String(Month)+"/"+String(Year)+
                ","+String(Fix)+","+String(FixQ)+","+String(NumSats)+","+String(Lat)+","+String(Longi)+","+String(Alti)+","+String(Temp)+","+String(Press);       
       

        Serial.println(Fdata); //Print Fdata to serial monitor

delay(1000);
        myFile.println(Fdata); //Write Fdata to file
        myFile.flush();
        writeCount++;


    }
  }
  
} 
  
bool newFile()
{   
  while (SD.exists(fileName))//increment file name
  {   Serial.print(fileName);
      fileNumber =fileNumber +1;
      tempstring =String("GDAT" + String(fileNumber)+".csv");
      tempstring.toCharArray(fileName,50);
      Serial.println(" already exist trying " +tempstring);
  } 
  myFile = SD.open(fileName, FILE_WRITE); //Open File
  //Serial.print ("File created");
  Serial.println(fileName);
 // myFile.println(file_header1);
  // myFile.println(file_header2);

  myFile.flush();
  writeCount=0;
  return true;
}

void GPS_config(){
/* Define data being Requested from GPS and at what rate */
GPS.sendCommand(PMTK_Q_RELEASE);
GPS.sendCommand(PGCMD_NOANTENNA);
GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

}


void useInterrupt(boolean v){
  //This function s used to enable and disable the interrupt.

  //millis() uses Timer0, so we will interrupt in and use this timer
/*******************************************************************/
  if (v) {
     OCR0A =0xAF;
     TIMSK0 |= _BV(OCIE0A);
     usingInterrupt = true;
  }
  else {
    TIMSK0 &= ~ _BV(OCIE0A);
    usingInterrupt = false;
  } 
}
/***************************Function to convert ADC to measured variable by applying the conversuin factor****/  
float getPressure(){

double temp_m =-0.25;
double temp_yintercept = 90;
double pressure_m = 0.0512;
double pressure_yintercept = -34.028;
return ((analogRead(A1)*pressure_m) + pressure_yintercept); //Return the result of the line equation mx+c
}
/***************************Function to convert ADC to measured variable by applying the conversuin factor****/  
float getTemperature(){

//double temp_m =-0.25;
//double temp_yintercept = 90;
//return ((analogRead(A0))*temp_m + temp_yintercept); //Return the result of the line equation mx+c

double m =-0.25;
double yintercept = 90;
//

//

return (   ( (analogRead(A0))*m  ) +  yintercept       ); //Return the result of the line equation mx+c



} 








































