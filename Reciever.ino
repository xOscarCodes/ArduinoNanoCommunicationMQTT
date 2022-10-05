
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS; 

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.mqttdashboard.com";
int port = 1883;
const char topic[] = "SIT210/wave";

bool recieve = false;

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(9600);
    pinMode(7, OUTPUT);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to WiFi network:
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    while (WiFi.begin(ssid, pass) != WL_CONNECTED)
    {
        // failed, retry
        Serial.print(".");
        delay(5000);
    }

    Serial.println("You're connected to the network");
    Serial.println();

    // You can provide a unique client ID, if not set the library uses Arduino-millis()
    // Each client must have a unique client ID
    // mqttClient.setId("clientId");

    // You can provide a username and password for authentication
    // mqttClient.setUsernamePassword("username", "password");

    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(broker);

    if (!mqttClient.connect(broker, port))
    {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());

        while (1)
            ;
    }

    Serial.println("You're connected to the MQTT broker!");
    Serial.println();

    Serial.print("Subscribing to topic: ");
    Serial.println(topic);
    Serial.println();

    // subscribe to a topic
    mqttClient.subscribe(topic);

    // topics can be unsubscribed using:
    // mqttClient.unsubscribe(topic);

    Serial.print("Waiting for messages on topic: ");
    Serial.println(topic);
    Serial.println();
}

void loop()
{
    int messageSize = mqttClient.parseMessage();
    if (messageSize)
    {
        // we received a message, print out the topic and contents
        Serial.print("Received a message with topic '");
        Serial.print(mqttClient.messageTopic());
        Serial.print("', length ");
        Serial.print(messageSize);
        Serial.println(" bytes:");

        recieve = false;
        
        // use the Stream interface to print the contents
        while (mqttClient.available())
        {
            Serial.print((char)mqttClient.read());
            
            recieve = true;
        }
        
        Serial.println();
        
        if (recieve)
        {
            blink();
        }
        
        Serial.println();
    }
}

void blink()
{
    Serial.println();
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(7, HIGH);
        delay(200);
        digitalWrite(7, LOW);
        delay(200);
    }
}
