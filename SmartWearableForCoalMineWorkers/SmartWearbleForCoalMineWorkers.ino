#include <SoftwareSerial.h>
// include the library code:
#include <LiquidCrystal.h>
#define USE_ARDUINO_INTERRUPTS true // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h> // Includes the PulseSensorPlayground Library.
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
#include <stdlib.h>
#include <dht.h>
#define dht_dpin A0 

int heart;
int i, j, k;
dht DHT;
long t1=0, t2=0, t3=0, t4=0, t5=0;
char buf1[16];
char buf2[16];
char buf3[16];
char buf4[16];
char buf5[16];
String strheart;
String strk;
String stri;
String strj;
// replace with your channel's thingspeak API key
String apiKey = "5B0NBSGUEBTRM3J3";


SoftwareSerial ser(5, 6); // RX, TX
// Variables
const int PulseWire = A1; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13; // The on-board Arduino LED, close to PIN 13.
int Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
// Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
// Otherwise leave the default "550" value.

PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
int myBPM;
// this runs once
void setup() {                
  // initialize the digital pin as an output.
   lcd.begin(16, 2);
lcd.setCursor(0, 0);
 lcd.print("Sensors");
 lcd.setCursor(0, 1);
 lcd.print("using IOT");
 delay(2000);
 lcd.clear();
analogReference(DEFAULT);
  // enable debug serial
  Serial.begin(9600);
  // enable software serial
  ser.begin(115200);
// reset ESP8266
  ser.println("AT+RST");
   delay(500);
  ser.println("AT+CWMODE=3");
   delay(500);
  ser.println("AT+CWJAP=\"project\",\"12345678\"");
  delay(500);

pinMode(2, OUTPUT);
digitalWrite(2, LOW);

// Configure the PulseSensor object, by assigning our variables to it.
pulseSensor.analogInput(PulseWire);
pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
pulseSensor.setThreshold(Threshold);

// Double-check the "pulseSensor" object was created and "began" seeing a signal.
if (pulseSensor.begin()) {
//Serial.println("We created a pulseSensor Object !"); //This prints one time at Arduino power-up, or on Arduino reset.
}

}
// the loop
void loop() {
   //Serial.println(analogRead(A3));
   DHT.read11(dht_dpin);
      
  

 i=DHT.humidity;
      j =DHT.temperature;
      j=j+5;
k=analogRead(A3);
 myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
// "myBPM" hold this BPM value now.
if (pulseSensor.sawStartOfBeat()) { // Constantly test to see if "a beat happened".
Serial.println("♥ A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
Serial.print("BPM: "); // Print phrase "BPM: "
Serial.println(myBPM/2.5); // Print the value inside of myBPM.
heart = myBPM/2.5;
}//heart

    lcd.setCursor(0, 0);
     lcd.print("HR:");
 lcd.print(heart);
 lcd.print("  ");
lcd.setCursor(8, 0);
    lcd.print("G: ");
     lcd.print(analogRead(A3));
 lcd.print("  ");
  lcd.setCursor(0, 1);
     lcd.print("H:");
 lcd.print(i);
 lcd.print("  ");
  lcd.setCursor(8, 1);
   lcd.print("T:");
 lcd.print(j);
 lcd.print(" ");

if(heart>100 || i>70 || j>40)
{
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
}
if(analogRead(A3)>600 || digitalRead(A2)==1)
  {
    
     
    digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  }
  
// convert to string
 
 strheart = dtostrf(heart, 4, 1, buf2);

  // convert to string

  stri = dtostrf(i, 4, 1, buf3);
strj = dtostrf(j, 4, 1, buf4);
  strk = dtostrf(k, 4, 1, buf5);
  
 
   Serial.print(strheart);
     Serial.print(" ");
    Serial.print(stri);
  Serial.print(" ");
   Serial.print(strj);
 Serial.print(" ");
   Serial.print(strk);
 Serial.print(" ");

  
     if(millis() - t2>15000)
  {
    t2=millis();
  
   // TCP connection
  String cmd1 = "AT+CIPSTART=\"TCP\",\"";
  cmd1 += "184.106.153.149"; // api.thingspeak.com
  cmd1 += "\",80";
  ser.println(cmd1);

  if(ser.find("Error")){
    //Serial.println("AT+CIPSTART error");
    return;
  }


 // prepare GET string
  String getStr1 = "GET /update?api_key=";
  getStr1 += apiKey;
  getStr1 +="&field1=";
  getStr1 += String(strheart);
  getStr1 += "\r\n\r\n";




// send data length
  cmd1 = "AT+CIPSEND=";
  cmd1 += String(getStr1.length());
  ser.println(cmd1);


   if(ser.find(">")){
    ser.print(getStr1);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  }
   if(millis() - t3>16000)
  {
    t3=millis();
  
   // TCP connection
  String cmd2 = "AT+CIPSTART=\"TCP\",\"";
  cmd2 += "184.106.153.149"; // api.thingspeak.com
  cmd2 += "\",80";
  ser.println(cmd2);

  if(ser.find("Error")){
    //Serial.println("AT+CIPSTART error");
    return;
  }


 // prepare GET string
  String getStr2 = "GET /update?api_key=";
  getStr2 += apiKey;
  getStr2 +="&field2=";
  getStr2 += String(stri);
  getStr2 += "\r\n\r\n";




// send data length
  cmd2 = "AT+CIPSEND=";
  cmd2 += String(getStr2.length());
  ser.println(cmd2);


   if(ser.find(">")){
    ser.print(getStr2);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }



  // thingspeak needs 15 sec delay between updates
  }
     if(millis() - t4>21000)
  {
    t4=millis();
  
   // TCP connection
  String cmd3 = "AT+CIPSTART=\"TCP\",\"";
  cmd3 += "184.106.153.149"; // api.thingspeak.com
  cmd3 += "\",80";
  ser.println(cmd3);

  if(ser.find("Error")){
    //Serial.println("AT+CIPSTART error");
    return;
  }


 // prepare GET string
  String getStr3 = "GET /update?api_key=";
  getStr3 += apiKey;
  getStr3 +="&field3=";
  getStr3 += String(strj);
  getStr3 += "\r\n\r\n";




// send data length
  cmd3 = "AT+CIPSEND=";
  cmd3 += String(getStr3.length());
  ser.println(cmd3);


   if(ser.find(">")){
    ser.print(getStr3);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

 

  // thingspeak needs 15 sec delay between updates
  }
  if(millis() - t5>9000)
  {
    t5=millis();
    
   // TCP connection
  String cmd4 = "AT+CIPSTART=\"TCP\",\"";
  cmd4 += "184.106.153.149"; // api.thingspeak.com
  cmd4 += "\",80";
  ser.println(cmd4);

  if(ser.find("Error")){
    //Serial.println("AT+CIPSTART error");
    return;
  }


 // prepare GET string
  String getStr4 = "GET /update?api_key=";
  getStr4 += apiKey;
  getStr4 +="&field4=";
  getStr4 += String(strk);
  getStr4 += "\r\n\r\n";




// send data length
  cmd4 = "AT+CIPSEND=";
  cmd4 += String(getStr4.length());
  ser.println(cmd4);


   if(ser.find(">")){
    ser.print(getStr4);
  }
   else{
    ser.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  }
 

}
