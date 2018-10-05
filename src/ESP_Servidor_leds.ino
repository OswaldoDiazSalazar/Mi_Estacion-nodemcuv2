#include <ESP8266WiFi.h>
#include <SimpleDHT.h>
int pinDHT11 = D0;
SimpleDHT11 dht11;
#include "Adafruit_BMP085.h"
#include "Wire.h"
#define P0 100800
Adafruit_BMP085 bmp;
const char ssid[] = "NOS-9D9E";
const char pass[] = "5VNL9AY6";
byte temperatura = 0;
byte humedad = 0;
float luminosidad = 0;
double T,P,A;
int val = 0;
char resultL[10];
char resultT[10];
char resultP[10];
char resultA[10]; 
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(A0, INPUT);
  // prepare GPIO2
  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  // preparando el sensor bmp180
  Wire.begin(0, 2);
  if(!bmp.begin()){
    Serial.println("BMP init failed!");
    while(1);
  }
  else Serial.println("BMP init success!");
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  //Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();
  
  // Match the request
  
  if (req.indexOf("/gpio/0") != -1){ 
    val = 0;
    // Set GPIO2 according to the request
    digitalWrite(4, val);
    leer_sensores();
    //respuesta();
    }
  else if (req.indexOf("/gpio/1") != -1){ 
    val = 1;
    // Set GPIO2 according to the request
    digitalWrite(4, val);
    leer_sensores();
    //respuesta();
    }
  else if (req.indexOf("/gpio/A") != -1){
    leer_sensores();
    //respuesta();
    }
 //else {
    //Serial.println("invalid request");
 //   client.stop();
 //   return;
 // }
// Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"OFF":"_ON";
  s += "</html>\n";

  // Send the response to the client
  client.flush();
  client.print(s);
  client.print("<P>");
  client.print("Temperatura: ");
  client.print(temperatura);
  //client.print("<P>");
  client.print(" Humedad: ");
  client.print(humedad);
  //client.print("<P>");
  client.print(" Luminosidad: ");
  client.print(resultL);
  client.print(" Temperatura2: ");
  client.print(resultT);
  client.print(" Presion Atmosferica: ");
  client.print(resultP);
  client.print(" Altura: ");
  client.print(resultA);
  delay(10);
  //Serial.println("Client disonnected");
 // client.flush();
 
 // client.stop();

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
void leer_sensores() {
  // procedimiento para leer los sensores
  luminosidad = analogRead(A0);
  //luminosidad = 100 - luminosidad;
  delay(300);
  if (dht11.read(pinDHT11, &temperatura, &humedad, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  }

  T = bmp.readTemperature();
  P = bmp.readPressure()/100;
  A = bmp.readAltitude(P0)-10;
 

  dtostrf(luminosidad, 8, 2, resultL);
  dtostrf(T, 8, 2, resultT);
  dtostrf(P, 8, 2, resultP);
  dtostrf(A, 8, 2, resultA);
  //    if(T !=0 )  {
  //      Serial.print("T = \t");Serial.print(T,2); Serial.print(" degC\t");
  //      Serial.print("P = \t");Serial.print(P,2); Serial.print(" mBar\t");
  //      Serial.print("A = \t");Serial.print(A,2); Serial.println(" m");
  //     
  //    }
  //    else {
  //      Serial.println("Error.");
  //
  //    }
}

//void respuesta() {
// Rutina para dar respuesta al cliente

//}
