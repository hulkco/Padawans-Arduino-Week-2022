# Padawans Arduino Week 2022

![Workshot logo](images/ponente/taller.png)


## Introducción.
Hoy en dia las aplicaciones basadas en el protocolo **WiFi**, son numerosas y bastantes difundidas, por lo tanto, existe mucha informacion de como crear una utlizando el ecosistema [Arduino](https://www.arduino.cc/) o similar, pero existe otro protocolo, el **Bluetooh Low Energy o BLE**, el cual como su nombre lo indica, esta diseñado para ser de bajo consumo y resuelve problemas donde WiFi puede no hacerlo, a diferencia de este ultimo, BLE no esta tan difundido y por ende, hay poda informacion, de como implementarlo.

![Nano BLE Sense](https://cdn.shopify.com/s/files/1/0506/1689/3647/products/ABX00031_03.front_622x467.jpg?v=1626445224)

Para este taller, se usara la tarjeta [Arduino Nano BLE Sense](https://docs.arduino.cc/hardware/nano-33-ble-sense), la cual ademas de tener conectividad BLE, cuanta con 5 sensores.

Sensor  		 | # de parte
------------- | -------------
IMU				 | LSM9DS
Microphone	 | MP34DT05
Gesture, light, proximity | APDS9960
Barometric pressure 	   | LPS22HB
Temperature, humidity		| HTS221

Con los cuales, se pueden crear una muy buena cantidad de aplicaciones/soluciones.

![](http://appinventor.mit.edu/images/logo.png)

![App](http://appinventor.mit.edu/explore/sites/all/files/ai2tutorials/wheresMyCar/WheresMyCarDesign.png)

En cuanto a la parte de ver/mostrar los datos, se usara App Inventor, el por que de esto, es muy sencillo, la rapidez de crear apps moviles sin necesidad de tener conocmientos bastos en programcion, como un servidor :wink:.

##Objetivo del taller.
Crear una solución BLE (Bluetooth Low Energy) basada en Arduino y App Inventor, para operar un grupo de leds RGB (Neopixeles)

### 1.Parte

**Elementos de una Aplicación BLE.**

Solo mencionare de forma breve dichos elementos, ya que el objetivo del taller es ver solo la parte practica, para profundizar, pueden ir a l apagina de la libreria [ArduinoBLE](https://www.arduino.cc/reference/en/libraries/arduinoble/).

Una aplicación BLE, a diferencia de una aplicacion **Bluetooht Clasic**, está optimizado para un uso de baja potencia a velocidades de datos bajas y fue diseñado para funcionar con baterías simples de litio tipo moneda, lo que lo hace muy atractivo para soluciones moviles, como monitorear los signos vitales en un persona.

La aplicacion, necesita de dos dispositivos, cada uno de llos debera cumplir con un rol bien definido que son:

**Dispositivos Centrales y Periféricos** 

**Centrales:** Los dispositivos centrales son clientes. leen y escriben datos de dispositivos periféricos. 

**Periféricos:** Los dispositivos periféricos son servidores. Proporcionan datos de sensores como características legibles y proporcionan la capacidad de lectura/escritura para controlar actuadores como motores, luces, etc. 

**Servicios, características y UUID**  

Un periférico Bluetooth® Low Energy brindará servicios, que a su vez brindarán características. Puede definir sus propios servicios o utilizar servicios [estándar](https://www.bluetooth.com/specifications/assigned-numbers/ ). 

Los servicios se identifican mediante números únicos conocidos como UUID ([generador de UUID](https://www.uuidgenerator.net/)). Los servicios estándar tienen un UUID de 16 bits y los servicios personalizados tienen un UUID de 128 bits. La capacidad de definir servicios y características depende de la radio(dispositivo) que esté utilizando y su firmware. 

Elementos de una aplicacion BLE en Arduino 

```
1. Estructura de un aplicación BLE

	Bliblioteca
	Identificador
	Caracteristica
	
	Setup
	```C++
	// Se establece el nombre y el servicio local a anunciarse.
  	BLE.setLocalName("LED");
  	BLE.setAdvertisedService(ledService);

  	// Añadir la característica al servicio
  	ledService.addCharacteristic(switchCharacteristic);

  	// Añadir el servicio
  	BLE.addService(ledService);

  	// establecer el valor inicial para la característica:
  	switchCharacteristic.writeValue(0);

  	// iniciar a anunciar el servicio
  	BLE.advertise();
  	```
	Loop


```
```C++
// Biblioteca BLE
#include <ArduinoBLE.h>

// Identificador (UUID) del servicio BLE "LED"
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
 
// Característica switchCharacteristic del servicio BLE "LED", usa un "UUID" personalizado de 128 bits, lectura y escritura centralizada
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// Pin asignado al servicio "LED"
const int ledPin = LED_BUILTIN; 

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

```
	





