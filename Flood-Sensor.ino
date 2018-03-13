// #include "Ticker-master/Ticker.h"
#include <SPI.h>
#include <SD.h>
#include "EngineeringMenu.h"
#include "Sensor.h"
#include "SDCard.h"
#include "Processor.h"
#include "Lorawan.h"
#define freqPlan TTN_FP_EU868

const byte sensorPin = 0;
const byte sensorPowerPin = 6;
const byte interruptPin = 12; //Engineering menu button jumper


Sensor sensor(sensorPin, sensorPowerPin);
SDCard sdCard;
Lorawan lorawan(Serial1, Serial, freqPlan);
Processor processor(&sensor, &sdCard, &lorawan, interruptPin);
EngineeringMenu menu(&sensor, &sdCard, &processor, &lorawan);

/**
 * Initialises default values and sets up Serial devices, pinModes and engineering menu interrupt.
 * @param N/A
 * @return {Void} N/A
 */
void setup()
{
  //  Setup serial baud, Serial1 used for LoRaWAN, Serial for USB communication.
  Serial1.begin(19200);
  Serial.begin(115200); 
  
  // Wait for serial to connect
  while(!Serial) {}
  Serial.println("USB Serial Setup");

  while(!Serial1){}
  Serial.println("LoRaWan Serial Setup");

  // initialize device
  processor.init();
  Serial.println("Device Initialized");

  pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), setBringUpMenu, CHANGE);
}


/**
 * Loop function to run indefinately.
 * Waits for delay period before attempting to take a river level measurement.
 * Or triggers engineering menu if interrupt function was triggered.
 * @param N/A
 * @return {Void} N/A
 */
void loop()
{
  
  
  // If button interrupt has been pressed
  if(digitalRead(interruptPin) == HIGH) {
    // Load the engineering menu
    menu.loadEngineeringMenu();
  }

  processor.readingProcess();

  processor.delayWithPeriod();
}
