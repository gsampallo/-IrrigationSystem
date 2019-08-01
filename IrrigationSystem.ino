#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* ssid = "WiFi";
const char* password = "password";
const char* mqtt_server = "broker IP";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

int pinValvula = 12;
int pinRelay1 = 14;

boolean estadoValvula = false;
unsigned long inicioRiego = 0; 
unsigned long finRiego = 5000; 

void setup() {
  Serial.begin(9600);
  Serial.println("iniciando");
  pinMode(pinValvula, OUTPUT);
  digitalWrite(pinValvula,HIGH);
  
  pinMode(pinRelay1, OUTPUT);
  digitalWrite(pinRelay1,HIGH);
  
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);   
}

void setup_wifi() {

  delay(10);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("OK");
}


void callback(char* topic, byte* payload, unsigned int length) {

  if (((char)payload[0] == '1')) {
    estadoValvula = true;
    inicioRiego = millis();
  } else if (((char)payload[0] == '2')) {
    Serial.println("Se detiene el riego");
    estadoValvula = false;
    digitalWrite(pinValvula,HIGH);
  }

}

void reconnect() {

  while (!client.connected()) {

    if (client.connect("IRRIGATION01")) {
      Serial.println("Conectado al broker");
      client.subscribe("MACETA");
    } else {
      delay(5000);
    }
  }
}

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 5000;    // the debounce time; increase if the output flickers

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 

  if(estadoValvula) {
    if ((millis() - inicioRiego) < finRiego) {
      digitalWrite(pinValvula,LOW);
      Serial.println("Regando");
    } else {
      digitalWrite(pinValvula,HIGH);
      estadoValvula = false;
      Serial.println("Se detiene el riego");
      client.publish("MACETA","2"); 
    }
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    int humedad = analogRead(A0);
    snprintf (msg, 75, "%ld", humedad);
    
    client.publish("HUMEDAD",msg);  
  }
}
