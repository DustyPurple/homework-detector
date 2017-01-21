#include <Event.h>
#include <Timer.h>

/*
  
  Parallax RFID Reader: Basic Demonstration       
                                                         
  Author: Joe Grand [www.grandideastudio.com]             
  Contact: support@parallax.com                            
  
  Program Description:
  
  This program provides a simple demonstration of the Parallax RFID Card
  Reader (#28140). If a valid RFID tag is placed in front of the reader,
  its unique ID is displayed in the Arduino Serial Monitor. 
  
  Please refer to the product manual for full details of system functionality 
  and capabilities.

  Revisions:
  
  1.0 (April 30, 2014): Initial release
  
*/

// include the SoftwareSerial library so we can use it to talk to the RFID Reader
#include <SoftwareSerial.h>

#define enablePin  13   // Connects to the RFID's ENABLE pin
#define rxPin      12  // Serial input (connects to the RFID's SOUT pin)
#define txPin      0  // Serial output (unused)

#define BUFSIZE    11  // Size of receive buffer (in bytes) (10-byte unique ID + null character)

#define RFID_START  0x0A  // RFID Reader Start and Stop bytes
#define RFID_STOP   0x0D

const int button1pin = 7;     // the number of the pushbutton pin
const int button2pin = 8;     // the number of the pushbutton pin
const int button3pin = 9;     // the number of the pushbutton pin
const int button4pin = 10;     // the number of the pushbutton pin
const int button5pin = 11;     // the number of the pushbutton pin

int buttonState = 1;         // variable for reading the pushbutton status
int hamstersLedPin= 6;//the number of the led pin
int grantsLedPin = 4;//the number of the led pin
int ritasLedPin = 3;//the number of the led pin
int norasLedPin = 2;//the number of the Led Pin
int dustysLedPin = 5;//the number of the led pin

//string for tags
const String ritasTag = "840034490F";
const String grantsTag = "35021DEDA9";
const String norasTag = "38003F8FA6";
const String dustysTag = "3501D5BFB0";
const String hamstersTag = "38005BC5BC";

long hour = (long) 1000 * 60 * 60; //1000 milli x 60 = 1 minute;  1 minute x 60 = 1 hour; 3,600,000 millis

//timer lengths
long ritasTimer = 17 * hour; //17 hour timer
long grantsTimer = 17 * hour;//17 hour timer
long norasTimer = 17 * hour;//17 hour timer
long dustysTimer = 10 * hour;// 10 hour timer
long hamstersTimer = 12 *hour;//12 hour timer

//Initialize users timer IDs
int ritasTimerId = -1;
int grantsTimerId = -1;
int norasTimerId = -1;
int dustysTimerId = -1;
int hamstersTimerId = -1;
Timer ledTimer; //timer object

// set up a new serial port
SoftwareSerial rfidSerial =  SoftwareSerial(rxPin, txPin);

void setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(enablePin, OUTPUT); //allows a signal to tansmit to the enable pin on the RFID reader
  pinMode(rxPin, INPUT); //allows a signal to be recived from the rfid reader
  pinMode(ritasLedPin, OUTPUT); //allows a signal to be sent to ritas led
  pinMode(grantsLedPin, OUTPUT);// allows a signal to be sent to grants led
  pinMode(norasLedPin, OUTPUT);// allows a signal to be sent to noras led
  pinMode(dustysLedPin, OUTPUT);// allows a signal to be sent to dustys led
  pinMode(hamstersLedPin, OUTPUT);// allows a signal to be sent to hamsters led
  pinMode(button1pin, INPUT_PULLUP);
  pinMode(button2pin, INPUT_PULLUP);
  pinMode(button3pin, INPUT_PULLUP);
  pinMode(button4pin, INPUT_PULLUP);
  pinMode(button5pin, INPUT_PULLUP);
    
  digitalWrite(enablePin, HIGH);  // disable RFID Reader
  
  // setup Arduino Serial Monitor
  Serial.begin(9600);
  while (!Serial);   // wait until ready
  Serial.println("\n\nParallax RFID Card Reader");
  Serial.println(ritasTimer);
  
  // set the baud rate for the SoftwareSerial port
  rfidSerial.begin(2400);

  Serial.flush();   // wait for all bytes to be transmitted to the Serial Monitor
}

void loop()  // Main code, to run repeatedly
{
  /* 
    When the RFID Reader is active and a valid RFID tag is placed with range of the reader,
    the tag's unique ID will be transmitted as a 12-byte printable ASCII string to the host
    (start byte + ID + stop byte)
    
    For example, for a tag with a valid ID of 0F0184F07A, the following bytes would be sent:
    0x0A, 0x30, 0x46, 0x30, 0x31, 0x38, 0x34, 0x46, 0x30, 0x37, 0x41, 0x0D
    
    We'll receive the ID and convert it to a null-terminated string with no start or stop byte. 
  */   
  
  digitalWrite(enablePin, LOW);   // enable the RFID Reader
  
  // Wait for a response from the RFID Reader
  // See Arduino readBytesUntil() as an alternative solution to read data from the reader
  char rfidData[BUFSIZE];  // Buffer for incoming data
  char offset = 0;         // Offset into buffer
  rfidData[0] = 0;         // Clear the buffer    

  //taken from example code EXCEPT for ledTimer.update();
  while(1)
  {
    ledTimer.update();  //update the timer while waiting for an rfid tag to be read 
    if (rfidSerial.available() > 0) // If there are any bytes available to read, then the RFID Reader has probably seen a valid tag
    {
      rfidData[offset] = rfidSerial.read();  // Get the byte and store it in our buffer
      if (rfidData[offset] == RFID_START)    // If we receive the start byte from the RFID Reader, then get ready to receive the tag's unique ID
      {
        offset = -1;     // Clear offset (will be incremented back to 0 at the end of the loop)
      }
      else if (rfidData[offset] == RFID_STOP)  // If we receive the stop byte from the RFID Reader, then the tag's entire unique ID has been sent
      {
        rfidData[offset] = 0; // Null terminate the string of bytes we just received
        break;                // Break out of the loop
      }
      offset++;  // Increment offset into array
      if (offset >= BUFSIZE) offset = 0; // If the incoming data string is longer than our buffer, wrap around to avoid going out-of-bounds
    }
    
    buttonState = digitalRead(button1pin);
    if (buttonState == LOW) 
    {
      digitalWrite(ritasLedPin, LOW);
      ritasTimerId = -1;
    } 
  }

  String rfidTagRead(rfidData); //making astring out of a charactor array

  if (ritasTag == rfidTagRead && ritasTimerId == -1)  //is it ritas tag
  {
    digitalWrite(ritasLedPin,HIGH);//turn the LED on
    ritasTimerId = ledTimer.after(ritasTimer, doAfterRitasLedPin); 
  }
  if (grantsTag == rfidTagRead && grantsTimerId == -1) //is it grants tag
  {
    digitalWrite(grantsLedPin,HIGH);//turn the LED on
    grantsTimerId = ledTimer.after(grantsTimer, doAfterGrantsLedPin);
    
  }
  if (norasTag == rfidTagRead && norasTimerId == -1) //is it noras tag
  {
    digitalWrite(norasLedPin,HIGH);//turn the LED on
    norasTimerId = ledTimer.after(10000, doAfterNorasLedPin);
 
  }
  if(dustysTag == rfidTagRead && dustysTimerId == -1) //is it dustys tag
   {
    digitalWrite(dustysLedPin,HIGH);//turn the LED on
    dustysTimerId = ledTimer.after(dustysTimer, doAfterDustysLedPin);
     
  }
  if(hamstersTag == rfidTagRead && hamstersTimerId == -1) //is it hamsters tag
  {
    digitalWrite(hamstersLedPin,HIGH);//turn the LED on
    hamstersTimerId = ledTimer.after(hamstersTimer, doAfterHamstersLedPin);
     
  }
  Serial.println(rfidData);       // The rfidData string should now contain the tag's unique ID with a null termination, so display it on the Serial Monitor
  Serial.flush();                 // Wait for all bytes to be transmitted to the Serial Monitor


}

void doAfterRitasLedPin()
{
  digitalWrite(ritasLedPin,LOW); //turn the LED off
  ritasTimerId = -1;
}

void doAfterGrantsLedPin()
{
  digitalWrite(grantsLedPin,LOW); //turn the LED off   
  grantsTimerId = -1;
}

void doAfterNorasLedPin()
{
  digitalWrite(norasLedPin,LOW); //turn the LED off 
  norasTimerId = -1;
}

void doAfterDustysLedPin()
{
digitalWrite(dustysLedPin,LOW); //turn the LED off
dustysTimerId = -1;
}

void doAfterHamstersLedPin()
{
  digitalWrite(hamstersLedPin,LOW); //turn the LED off
  hamstersTimerId = -1;
}
