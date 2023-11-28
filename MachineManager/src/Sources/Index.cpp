#include "./Headers/Index.h"

const char Index::webPage[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<script>

var connection = new WebSocket('ws://'+location.hostname+':81/');

var button_1_status = 0;
var button_2_status = 0;
var slider_val = 0;
var sens_data = 0;
connection.onmessage = function(event){
  var full_data = event.data;
  console.log(full_data);
  var data = JSON.parse(full_data);
  sens_data = data.sens;
  document.getElementById("sensor_meter").value = sens_data;
  document.getElementById("sensor_value").innerHTML = sens_data;
}

function updateSlider(slideAmount) {
  var sliderDiv = document.getElementById("sliderAmount");
  sliderDiv.innerHTML = slideAmount;
  slider_val = slideAmount;
  console.log("Slider Amount is" + slideAmount);
  send_data();
}

function button_1_on()
{
  button_1_status = 0; 
  console.log("LED 1 is ON");
  send_data();
}

function button_1_off()
{
  button_1_status = 1;
  console.log("LED 1 is OFF");
  send_data();
}

function button_2_on()
{
  button_2_status = 0; 
  console.log("LED 2 is ON");
  send_data();
}

function button_2_off()
{
  button_2_status = 1;
  console.log("LED 2 is OFF");
  send_data();
}

function send_data()
{
  var full_data = '{"LED1" :'+button_1_status+', "LED2":'+button_2_status+', "PWM1":'+slider_val+'}';
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
        <p class="card-title"><i class="fas fa-tint"></i> BOARD #1 - MAIN MOTOR</p><p>
        <div class="slidecontainer">
          <p>Default range slider:</p>
          <input type="range" min="0" max="255" value="0" step="1" onchange="updateSlider(this.value)">
        </div>
        <div id="sliderAmount"></div>
        </p><p class="timestamp">Last Reading: <span id="rh1"></span></p>
      </div>
    </div>
  </div>
  <div style="text-align: center;">
  <h3>Proximity Sensor</h3><meter value="0" min="0" max="1" id="sensor_meter"> </meter><h3 id="sensor_value" style="display: inline-block;"> 2 </h3>
</body>
</html>
)=====";

const char* Index::getIndexPage()
{
    return webPage;
}