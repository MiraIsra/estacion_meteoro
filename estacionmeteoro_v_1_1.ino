#include "DHT.h"
#include <WiFiEsp.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BME280.h"
#include "avr/wdt.h"


#define WIFI_AP "Casa"
#define WIFI_PASSWORD ""

#define esp_tx_pin 3 //6
#define esp_rx_pin 2 //5
#define esp_power_pin 5


#define TOKEN ""

const int estacion = 1; // Identificador de la estación actual
//**********Declaramos pines de sensores**********//
// DHT
#define DHTPIN 4
#define DHTTYPE DHT11
// Fotoresistor
const int LDRPin = A0;
const long A = 16;     // Resistencia en oscuridad en KΩ LA RESISTENCIA QUE COLOCAMOS EN EL ESQUEMA ELECTRICO
const int B = 15;        // Resistencia a la luz (10 Lux) en KΩ**DATO A CALIBRAR**
const int Rc = 10;       // Resistencia calibracion en KΩ**DATO A CALIBRAR**


char mqtt_server[] = "";

// Initialize the Ethernet client object
WiFiEspClient espClient;
const char* topicName = "";

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

PubSubClient client(espClient);
// Declaramos libreria presion barométrica
Adafruit_BME280 bmp; // I2C
 
SoftwareSerial soft(esp_rx_pin, esp_tx_pin); // RX, TX (2, 3) (5, 6)

int status = WL_IDLE_STATUS;
unsigned long lastSend;

String GetSensorData();
void SendSensorData (String);

void setup() {
  // Inicializamos Wifi
  Serial.begin(9600);
  //Configuro watchdog
  wdt_disable(); // deshabilito el watchdog
  delay(1000);
  wdt_enable (WDTO_8S); // habilito el watchdog cada 8 segundos
  Serial.println(F("Watchdog enabled."));
  int t = millis();
 
  InitWiFi();
  Serial.print("InitWifi: ");
  Serial.println(millis()-t);
  
  client.setServer(mqtt_server, 503); // Puerto mqtt
  lastSend = -1800000;

  //*****   Inicializamos sensores *****//
  dht.begin();
  bmp.begin(); // Inicia el sensor
  pinMode(LDRPin, INPUT);
}

void loop() {
  //*****   Conexion wifi *****//
  status = WiFi.status();
  if ( status != WL_CONNECTED ) {
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(WIFI_AP);
      // Connect to WPA/WPA2 network
      wdt_enable (WDTO_8S); // habilito el watchdog
      status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      wdt_disable(); // Deshabilito el watchdog
      delay(1000);
    }
    Serial.println("Connected to AP");
  }
  
  if ( !client.connected() ) {
    reconnect();
  }

//***** Chequeamos el tiempo para leer / mandar datos *****//
  /*if (millis() - lastRead > 90000) // Update sensor data every 15 minute: 600000
  {
    sensorData = GetSensorData();
    lastRead = millis();
  }*/
  if ( millis() - lastSend > 895500 ) { // Send only after 15 min = 900000 // Ajustamos a 14min 55.5 seg. = 895500
    
    String sensorData = GetSensorData();
    //delay(50);
    SendSensorData(sensorData);
    Serial.println("Enviado.");
  }
  

  client.loop();
}

String GetSensorData ()
{
  //Serial.println("Collecting data...");

  // Reading presure sensor data
  float p = bmp.readPressure()/100; // Almacena la presion atmosferica (Pa)
  float t1 = bmp.readTemperature(); // Almacena la temperatura (oC)
  float hrel = bmp.readHumidity();
  int a = bmp.readAltitude (1016.5);
  //delay(50);
  // Reading temperature or humidity takes about 250 milliseconds!
  float habs = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dht.readTemperature();
  //delay(50);
  // Reading fotoresistor sensor value
  float vf = analogRead(LDRPin);
  float l = (100 - vf/10)-10;//((long)vf*A*10)/((long)B*Rc*(1024-vf));//(100 - vf/10)-10;//
  if (l < 0 || l > 87)
    l = l + 10;
  //delay(50);
  
  // Check if any reads failed and exit early (to try again).
  
  if (isnan(habs) || isnan(t1) || isnan(t2) || isnan(hrel) || isnan(a) || isnan(p) || isnan(vf)) {
    Serial.println("Failed to read from sensors!");
    return;
  }
  
  // Obtenemos media de ambas T
  float t = (t1 + t2)/2;  // Hacemos la media de las dos temperaturas obtenidas
  
  String temperature = String(t);
  String humidityAbs = String(habs);
  String humidityRel = String(hrel);
  String presion = String(p);
  String altura = String(a);
  String luminance = String (l);

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"est\":"; payload += estacion; payload += ",";
  payload += "\"temp\":"; payload += temperature; payload += ",";
  payload += "\"humRel\":"; payload += humidityRel; payload += ",";
  payload += "\"humAbs\":"; payload += humidityAbs; payload += ",";
  payload += "\"lum\":"; payload += luminance; payload += ",";
  payload += "\"pres\":"; payload += presion; payload += ",";
  payload += "\"alt\":"; payload += altura; payload += ",";
  payload += "}";

  //Serial.println(payload);
  
  return payload;
}

void SendSensorData(String payload)
{
  // Just debug messages
  Serial.println( "[ " + payload + " ]" );
  
  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( TOKEN, attributes );
  Serial.println( attributes );
  lastSend = millis();
}

void InitWiFi()
{ 
  soft.begin(9600);
  // initialize ESP module
  
  WiFi.init(&soft);
  wdt_reset();
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  wdt_reset();
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_AP);
    // Connect to WPA/WPA2 network
    wdt_reset();
    status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    wdt_reset();
    delay(5000);
  }
  wdt_disable(); // deshabilito el watchdog
  Serial.println("Connected to AP");
  delay(50);
}

void reconnect() {
  // Loop until we're reconnected
  int intentos = 0;
  wdt_enable (WDTO_8S); // habilito el watchdog
  while (!client.connected() && intentos < 10) {
    Serial.print("Connecting to miraraspiisra node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("Arduino Uno Device", topicName, NULL) ) {
      wdt_disable(); // Deshabilito el watchdog
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 30 seconds]" );
      intentos += 1;
      // Wait 30 seconds before retrying
      delay( 5000 );
    }
  }
}
