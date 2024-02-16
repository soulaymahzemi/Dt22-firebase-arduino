#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#include <WiFi.h>

#include <ESP32Servo.h> 

#define FIREBASE_HOST "esp32-4f910-default-rtdb.europe-west1.firebasedatabase.app/"
#define FIREBASE_AUTH "snT7RPFr1AU1BsoB4fFQqPsmYOivFflKNPHHCmTJ"
FirebaseData firebaseData;


Servo myservo;  // create servo object to control a servo

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  myservo.attach(13);
/*
  WiFi.disconnect();
  delay(1000);
  Serial.println("START");
  */
  WiFi.begin("Flybox_14B","810044258");
  /*
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }*/

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
}

void loop(){
  
  delay(8000);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
   Serial.println(F("Failed to read from DHT sensor!"));
   return;
  }


   // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);  //Fahrenheit
  Serial.print(F("°C "));
  Serial.print(hif);  //Celsius
  Serial.println(F("°F"));


  delay(2000);
  
    if ((Firebase.setFloat(firebaseData, "Humidity", (h))== true) ){
      Serial.println("Humidity send");

    } else {
      Serial.println("Error :(");

    }

     if ((Firebase.setFloat(firebaseData, "Temperature", (t))== true) ){
      Serial.println("Temperature send");

    } else {
      Serial.println("Error :(");

    }

   if((t<26)||(h>16)){
    myservo.write(90);                  
  delay(1000);    
  } else {
  myservo.Wwrite(180);                  
  delay(1000); 
   }
   }
  
}
