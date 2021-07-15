

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Dexter";
const char* password = "32649971";
const char* mqtt_server = "192.168.1.62";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
char data[MSG_BUFFER_SIZE];
int value = 0;
int count = 0;
  int httpCode;
  int stateAlarm;
  int SENSOR = 2;
  int ALARMA = 4;
  int SECURITY_DOORS = 14;
  int DESACTIVER = 12;
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print("Topic MQTT callBack >>>");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print("Topic MQTT callBack >>>");
    Serial.print((char)payload[i]);
  }
//  Serial.print((char)payload);

  if ((char)payload[0] == 'C') {
    digitalWrite(SECURITY_DOORS, HIGH);  
    digitalWrite(BUILTIN_LED, LOW);
  } else {
    digitalWrite(SECURITY_DOORS, LOW);  
    digitalWrite(BUILTIN_LED, HIGH); 
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    //  client.publish("outTopic", "hello world");
     // client.publish("alarmTopic", "Alarm_Is_Fire");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(DESACTIVER, INPUT_PULLUP);
  pinMode(ALARMA, OUTPUT);
  pinMode(SECURITY_DOORS, OUTPUT);

  digitalWrite(SECURITY_DOORS, LOW);
  digitalWrite(ALARMA, LOW);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
   int pushButtonState;
    int desactiveState;
    pushButtonState = digitalRead(SENSOR);
    desactiveState = digitalRead(DESACTIVER);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
 //   client.publish("outTopic", msg);
  }
  
  if(pushButtonState == LOW){
    ++count;
    snprintf (data, MSG_BUFFER_SIZE, "Alarm_Is_Fire", count);
    Serial.println(data);
    digitalWrite(ALARMA, HIGH);
    client.publish("alarmTopic", data);
    delay(1000);
  }else{
     if(desactiveState == LOW){
       digitalWrite(ALARMA, LOW);   
       digitalWrite(SECURITY_DOORS, LOW);
       snprintf (data, MSG_BUFFER_SIZE, "System_reset", count);
       client.publish("alarmTopic", data);    
       Serial.println("All Is Safe!!");
      }
    }
}
