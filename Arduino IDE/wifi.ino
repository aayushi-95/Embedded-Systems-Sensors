char* ssid = "office_net";
char* password = "bosco123";
#include <ESP8266WiFi.h>

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  while(WiFi.status()!=WL_CONNECTED)
  {
    
    Serial.print(".");
   pinMode(14,OUTPUT);
   digitalWrite(14,HIGH);
    delay(500);
    digitalWrite(14,LOW);
    delay(500);
  }

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());

   
  
  WiFi.mode(WIFI_STA);

    
}
void loop() {
  // put your main code here, to run repeatedly:
  
    while(WiFi.status()==WL_CONNECTED)
 { 
    digitalWrite(14,HIGH);
    Serial.println("@");
    delay(3000);
    digitalWrite(14,LOW);
    delay(3000);
}
}
