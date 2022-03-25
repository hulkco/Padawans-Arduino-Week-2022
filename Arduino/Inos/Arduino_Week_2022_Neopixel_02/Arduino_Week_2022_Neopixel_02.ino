/****************************************************************************
 * Aplicacion para el control de una matrix de Neopixeles
 * usando BLE con Arduino Nano Sense
 * Para: Arduino Week 2022
 * Por: Gustavo Reynaga @gsreynaga
 * 
 * This example code is in the public domain.
 * ***************************************************************************
 */
 // BibliotacaBLE
#include <Adafruit_NeoPixel.h>
// Pin donde esta conectado la matriz/tira de Neopixels
#define PIN    12 
// Cuantos Led tiene la matriz/tira?
#define LED_COUNT  8
// Cantidad de brillo, 0 (min) a 255 (max)
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

#include "efectos.h"

int red;
int green;
int blue;

//uint8_t rgb_values[3]; //comentada para descubir el uso

#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic redCharacteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic greenCharacteristic("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic blueCharacteristic("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);


void setup()
{
    Serial.begin(115200);
    Serial.println("Hello World!");

    // custom services and characteristics can be added as well
    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting BLE failed!");

        while (1);
    }

    Serial.print("Peripheral address: ");
    Serial.println(BLE.address());

    // set advertised local name and service UUID:
    BLE.setLocalName("NEOPIXEL");
    BLE.setAdvertisedService(ledService);

    // add the characteristic to the service
    ledService.addCharacteristic(switchCharacteristic);
    ledService.addCharacteristic(redCharacteristic);
    ledService.addCharacteristic(greenCharacteristic);
    ledService.addCharacteristic(blueCharacteristic);



    // add service
    BLE.addService(ledService);

    // set the initial value for the characeristic:
    switchCharacteristic.writeValue(10);
    Serial.print("switch = ");
    Serial.println(switchCharacteristic);
    redCharacteristic.writeValue(255);
    greenCharacteristic.writeValue(0);
    blueCharacteristic.writeValue(0); 
    

    // start advertising
    BLE.advertise();

    strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();  // Turn OFF all pixels ASAP

    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);

}

void loop()
{
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central)
    {
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());

        // while the central is still connected to peripheral:
        while (central.connected())
        {
            // if the remote device wrote to the characteristic,
            // use the value to control the LED:
            if (switchCharacteristic.written())
            {                  
                if (switchCharacteristic.value() == 1){
                  rainbow(10);
                }

                if (switchCharacteristic.value() == 2){                  
                  colorWipe(strip.Color(redCharacteristic.value(), greenCharacteristic.value(), blueCharacteristic.value()), 20);
                  Serial.println("RGB");
                  Serial.print(redCharacteristic.value());
                  Serial.print(",");
                  Serial.print(greenCharacteristic.value());
                  Serial.print(",");
                  Serial.println(blueCharacteristic.value()); 
                }

                if (switchCharacteristic.value() == 10){
                  colorWipe(strip.Color(0, 0, 0), 20); // Black  
                  clearStrip;
                }
            }
            
        }
    }
}
