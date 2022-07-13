 #include "DHT.h"
#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 bmp180;
DHT dht(3, DHT11);
#define sensor 2
int REF_3V3 = A1;
int UVOUT = A0;
byte v_sensor = 1;

void setup() {
  pinMode(sensor, INPUT);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  Serial.begin(9600);
  dht.begin();
  if (bmp180.begin())
    Serial.println("");
  else
  {
    Serial.println("Error al iniciar el BMP180");
    while(1); // bucle infinito
  }
}

void loop() {
  char status;
  double T,P;
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0);
  if(Serial.available()>0)
  {
    char data=Serial.read();
    switch(data)
    {
      case'1':
        Serial.print("Estado: ");
        v_sensor = digitalRead(sensor);
        if(v_sensor == 0) {
          Serial.println("Lluvia!");
       }
       else {
          Serial.println("Sol!");
       }
      break;
      
      case'2':
        //Rayos UV
        Serial.print("Intensidad UV(mW/cm^2): ");
        Serial.println(uvIntensity);
      break;
      
      case'3':
        if (isnan(h) || isnan(t)) {
        Serial.println("Falla al leer el sensor DHT11!");
        return;
      }
 
        // IMPRIMIR RESULTADO AL MONITOR SERIAL
        Serial.print("Humedad: ");
        Serial.print(h);
        Serial.println(" % ");
      break;
      case'4':
        status = bmp180.startTemperature();//Inicio de lectura de temperatura
        if (status != 0)
        {   
          delay(status); //Pausa para que finalice la lectura
          status = bmp180.getTemperature(T); //Obtener la temperatura
          if (status != 0)
          {
            status = bmp180.startPressure(3); //Inicio lectura de presión
            if (status != 0)
            {        
              delay(status);//Pausa para que finalice la lectura        
              status = bmp180.getPressure(P,T); //Obtenemos la presión
              if (status != 0)
              {                  
                Serial.print("Presion: ");
                Serial.print(P,2);
                Serial.println(" mbar");          
              }      
            }      
          }   
        }
      break;
      case'5':
        Serial.print("Temperatura: ");
        Serial.print(t);
        Serial.println(" *C");
      break;
      
      case'0':
        //Sensor de lluvia
        Serial.print("Estado: ");
        v_sensor = digitalRead(sensor);
        if(v_sensor == 0) {
          Serial.println("Lluvia!");
        }
        else {
          Serial.println("Sol!");
        }
        //Rayos UV
        Serial.print("Intensidad UV(mW/cm^2): ");
        Serial.println(uvIntensity);
        //sensor de humedad
        // REVISAR QUE LOS RESULTADOS SEAN VALORES NUMERICOS VALIDOS, INDICANDO QUE LA COMUNICACION ES CORRECTA
        if (isnan(h) || isnan(t)) {
          Serial.println("Falla al leer el sensor DHT11!");
          return;
        }
        // IMPRIMIR RESULTADO AL MONITOR SERIAL
        Serial.print("Humedad: ");
        Serial.print(h);
        Serial.println(" % ");
        //Sensor de presion
        status = bmp180.startTemperature();//Inicio de lectura de temperatura
        if (status != 0)
        {   
          delay(status); //Pausa para que finalice la lectura
          status = bmp180.getTemperature(T); //Obtener la temperatura
          if (status != 0)
          {
            status = bmp180.startPressure(3); //Inicio lectura de presión
            if (status != 0)
            {        
              delay(status);//Pausa para que finalice la lectura        
              status = bmp180.getPressure(P,T); //Obtenemos la presión
              if (status != 0)
              {                  
                Serial.print("Presion: ");
                Serial.print(P,2);
                Serial.println(" mbar");          
              }      
            }      
          }   
        }
        Serial.print("Temperatura: ");
        Serial.print(T);
        Serial.println(" *C");
        break;
     }
     Serial.println(data);
    }
    delay(50); 
}
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
  for(int x = 0 ; x < numberOfReadings ; x++)
  runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return(runningValue);  
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
