/*
  LED

  Este ejemplo crea un periférico BLE con un Servicio que contiene una
  característica para controlar un LED.

  This example code is in the public domain.
*/
// Biblioteca BLE
#include <ArduinoBLE.h>

// Identificador (UUID) del servicio BLE "LED"
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
 
// Característica switchCharacteristic del servicio BLE "LED", usa un "UUID" personalizado de 128 bits, lectura y escritura centralizada
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// Pin asignado al servicio "LED"
const int ledPin = 2; 

//Inicia funcion "SETUP"
void setup() {
  Serial.begin(9600); //Iniciamos el servicio de "escucha en la consola serial."  

  // Se configura el pin asignado al servicio "LED" como salida digital.
  pinMode(ledPin, OUTPUT);  

  // Se inicializa verificacion que el servicio "BLE" este activo, en caso 
  // contrario manda un mensaje a la consola serial
  if (!BLE.begin()) {
    Serial.println("Servicio BLE ha fallado!");

    while (1);
  }

  // Se establece el nombre y el servicio local a anunciarse.
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // añadir la característica al servicio
  ledService.addCharacteristic(switchCharacteristic);

  // añadir el servicio
  BLE.addService(ledService);

  // establecer el valor inicial para la característica:
  switchCharacteristic.writeValue(0);

  // iniciar el advertising (anunciar el servicio)
  BLE.advertise();
  // imprime en consola
  Serial.println("BLE LED Peripheral");
}

// Inicia la funcion LOOP
void loop() {
  // Esperando conexiones
  BLEDevice central = BLE.central();

  // si un dispositivo central se conecta al to periferico:
  if (central) {
    Serial.print("Conectedo a la central: ");
    // Imprime la direccion MAC:
    Serial.println(central.address());

    // mientras el dispositivo central sigue conectado al periférico:
    while (central.connected()) {
      // si el dispositivo remoto escribió en la característica,
      // se usa el valor recibido para controlar el LED:
      if (switchCharacteristic.written()) { // si la propiedad recibe datos
        if (switchCharacteristic.value() == 1) { // si el valor es igual a "1"  
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // enciende el LED
        }
        if (switchCharacteristic.value() == 0) { // si el valor es igual a "0"    
          Serial.println("LED off");
          digitalWrite(ledPin, LOW);         // apaga el LED
        }
      }
    }

    // cuando la central se desconecte:
    Serial.print(F("Dispositivo desconectado..."));
    Serial.println(central.address());
  }
}
