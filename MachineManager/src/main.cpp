#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif

#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Ticker.h> //https://github.com/sstaub/Ticker

#include <ESPAsyncWebServer.h>
#include <espnow.h>

AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);


#define LED1 13
#define LED2 12


void send_sensor();
Ticker timer;



// --------------------------------------
// Insert your SSID
constexpr char WIFI_SSID[] = "esp_server";//"CORREA_2.4GHz";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i=0; i<n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}

//MAC Address of the receiver 
uint8_t broadcastAddress[] = {0x48, 0x3F, 0xDA, 0xAA, 0x51, 0xCF}; // 48:3F:DA:AA:51:CF

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int led_val;
} struct_message;

//Create a struct_message called myData
struct_message myData;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;


// callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) { 
  // Copies the sender mac address to a string
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
  // board["id"] = incomingReadings.id;
  // board["temperature"] = incomingReadings.temp;
  // board["humidity"] = incomingReadings.hum;
  // board["readingId"] = String(incomingReadings.readingId);
  // String jsonString = JSON.stringify(board);
  // events.send(jsonString.c_str(), "new_readings", millis());

  digitalWrite(LED_BUILTIN, incomingReadings.led_val);

  Serial.printf("LED received value: %d \n", incomingReadings.led_val);
  Serial.println();
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}


// --------------------------------------







char webpage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<script>

var connection = new WebSocket('ws://'+location.hostname+':81/');

var button_1_status = 0;
var button_2_status = 0;
var sens_data = 0;
connection.onmessage = function(event){

  var full_data = event.data;
  console.log(full_data);
  var data = JSON.parse(full_data);
  sens_data = data.sens;
  document.getElementById("sensor_meter").value = sens_data;
  document.getElementById("sensor_value").innerHTML = sens_data;
}

function button_1_on()
{
   button_1_status = 1; 
  console.log("LED 1 is ON");
  send_data();
}

function button_1_off()
{
  button_1_status = 0;
console.log("LED 1 is OFF");
send_data();
}

function button_2_on()
{
   button_2_status = 1; 
  console.log("LED 2 is ON");
  send_data();
}

function button_2_off()
{
  button_2_status = 0;
console.log("LED 2 is OFF");
send_data();
}

function send_data()
{
  var full_data = '{"LED1" :'+button_1_status+',"LED2":'+button_2_status+'}';
  connection.send(full_data);
}


</script>

<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h1 {  font-size: 2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(320px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .timestamp { color: #bebebe; font-size: 1rem; }
    .card-title{ font-size: 1.2rem; font-weight : bold; }
    .card.temperature { color: #B10F2E; }
    .card.humidity { color: #50B8B4; }
  </style>
</head>

<body>
<div class="topnav">
    <h1>ESP-NOW DASHBOARD</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <p class="card-title"><i class="fas fa-thermometer-half"></i> BOARD #1 - BUILTIN LED</p><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="timestamp">Last Reading: <span id="rt1"></span></p>
        <button onclick= "button_1_on()" >On</button><button onclick="button_1_off()" >Off</button>
      </div>
      <div class="card humidity">
        <p class="card-title"><i class="fas fa-tint"></i> BOARD #1 - MAIN MOTOR</p><p><span class="reading"><span id="h1"></span> &percnt;</span></p><p class="timestamp">Last Reading: <span id="rh1"></span></p>
      </div>
    </div>
  </div>
  <div style="text-align: center;">
  <h3>Proximity Sensor</h3><meter value="0" min="0" max="1" id="sensor_meter"> </meter><h3 id="sensor_value" style="display: inline-block;"> 2 </h3>
</body>
</html>
)=====";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);

      
    DynamicJsonDocument doc(200);
    DeserializationError error = deserializeJson(doc, message);
    // parse the parameters we expect to receive (TO-DO: error handling)
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    // int LED1_status = doc["LED1"];
    // int LED2_status = doc["LED2"];
    // digitalWrite(LED_BUILTIN,LED1_status);
    // digitalWrite(LED2,LED2_status);

    // --------------------------------------------
    myData.led_val = doc["LED1"];
      
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    Serial.println("Assync package sent");

  }
}

void setup(void)
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  WiFi.softAP("esp_server", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());


  if (MDNS.begin("esp")) { //esp.local/ //
    Serial.println("MDNS responder started");
  }

  server.on("/", [](AsyncWebServerRequest * request)
  { 
   request->send_P(200, "text/html", webpage);
  });

  server.onNotFound(notFound);

  server.begin();  // it will start webserver
  websockets.begin();
  websockets.onEvent(webSocketEvent);
  // timer.attach(0.5,send_sensor);
  pinMode(D5,INPUT);


  // ------------------------------------
  // Set device as a Wi-Fi Station and set channel
  // WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  wifi_promiscuous_enable(1);
  wifi_set_channel(channel);
  wifi_promiscuous_enable(0);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}


void loop(void)
{
 websockets.loop();
}

void send_sensor()
{
   int sens = digitalRead(D5);
  // JSON_Data = {"temp":t,"hum":h}"
  String JSON_Data = "{\"sens\":";
         JSON_Data += sens;
         JSON_Data += "}";
   Serial.println(JSON_Data);     
  websockets.broadcastTXT(JSON_Data);
}
