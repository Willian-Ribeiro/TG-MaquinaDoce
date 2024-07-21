#include "./Headers/Index.h"

// const char Index::webPage[] PROGMEM = R"=====(

// <!DOCTYPE html>
// <html>

// <script>

// var connection = new WebSocket('ws://'+location.hostname+':81/');

// var button_1_status = 0;
// var button_2_status = 0;
// var slider_val = 0;
// var sens_data = 0;
// connection.onmessage = function(event){
//   var full_data = event.data;
//   console.log(full_data);
//   var data = JSON.parse(full_data);
//   sens_data = data.sens;
//   document.getElementById("sensor_meter").value = sens_data;
//   document.getElementById("sensor_value").innerHTML = sens_data;
// }

// function updateSlider(slideAmount) {
//   var sliderDiv = document.getElementById("sliderAmount");
//   sliderDiv.innerHTML = slideAmount;
//   slider_val = slideAmount;
//   console.log("Slider Amount is" + slideAmount);
//   send_data();
// }

// function button_1_on()
// {
//   button_1_status = 0; 
//   console.log("LED 1 is ON");
//   send_data();
// }

// function button_1_off()
// {
//   button_1_status = 1;
//   console.log("LED 1 is OFF");
//   send_data();
// }

// function button_2_on()
// {
//   button_2_status = 0; 
//   console.log("LED 2 is ON");
//   send_data();
// }

// function button_2_off()
// {
//   button_2_status = 1;
//   console.log("LED 2 is OFF");
//   send_data();
// }

// function send_data()
// {
//   var full_data = '{"LED1" :'+button_1_status+', "LED2":'+button_2_status+', "PWM1":'+slider_val+'}';
//   connection.send(full_data);
// }


// </script>

// <head>
//   <title>ESP-NOW DASHBOARD</title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
//   <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
//   <link rel="icon" href="data:,">
//   <style>
//     html {font-family: Arial; display: inline-block; text-align: center;}
//     h1 {  font-size: 2rem;}
//     body {  margin: 0;}
//     .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
//     .content { padding: 20px; }
//     .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
//     .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(320px, 1fr)); }
//     .reading { font-size: 2.8rem; }
//     .timestamp { color: #bebebe; font-size: 1rem; }
//     .card-title{ font-size: 1.2rem; font-weight : bold; }
//     .card.temperature { color: #B10F2E; }
//     .card.humidity { color: #50B8B4; }
//   </style>
// </head>

// <body>
// <div class="topnav">
//     <h1>ESP-NOW DASHBOARD</h1>
//   </div>
//   <div class="content">
//     <div class="cards">
//       <div class="card temperature">
//         <p class="card-title"><i class="fas fa-thermometer-half"></i> BOARD #1 - BUILTIN LED</p><p><span class="reading"><span id="t1"></span> &deg;C</span></p><p class="timestamp">Last Reading: <span id="rt1"></span></p>
//         <button onclick= "button_1_on()" >On</button><button onclick="button_1_off()" >Off</button>
//       </div>
//       <div class="card humidity">
//         <p class="card-title"><i class="fas fa-tint"></i> BOARD #1 - MAIN MOTOR</p><p>
//         <div class="slidecontainer">
//           <p>Default range slider:</p>
//           <input type="range" min="0" max="255" value="0" step="1" onchange="updateSlider(this.value)">
//         </div>
//         <div id="sliderAmount"></div>
//         </p><p class="timestamp">Last Reading: <span id="rh1"></span></p>
//       </div>
//     </div>
//   </div>
//   <div style="text-align: center;">
//   <h3>Proximity Sensor</h3><meter value="0" min="0" max="1" id="sensor_meter"> </meter><h3 id="sensor_value" style="display: inline-block;"> 2 </h3>
// </body>
// </html>
// )=====";

const char Index::webPageUpdated[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>

<script>

class Data {
  dataSource;
  operating = false;
  operationTime = 0;
  motorSpeed1 = 0;
  motorSpeed2 = 0;

  constructor(dataSource, operating) {
    this.dataSource = dataSource;
  }
}

const DataSources = {
  MIXER: "mixer",
  MODELER: "modeler",
  GRANULATOR: "granulator",
};

var mixerData = new Data(DataSources.MIXER);
var modelerData = new Data(DataSources.MODELER);
var granulatorData = new Data(DataSources.GRANULATOR);

// send data
var operation_time = 0; //  in seconds
var motor_speed = 0; // SPEED_OFF:0, SPEED_V1:1, SPEED_V2:2

// auxiliar data
var slider_val = 0;

// connection
var connection = new WebSocket('ws://'+location.hostname+':81/');

// Sending data to NODE_MCU ---------------------------------------------------------
function send_data(data)
{
  var timeMillis = operation_time * 1000;
  
  var full_data = '{"DATA_SOURCE":"'+data.dataSource+'", "OPERATING":'+data.operating+', "OPERATION_TIME":'+data.operationTime+', "MOTOR_SPEED1":'+data.motorSpeed1+', "MOTOR_SPEED2":'+data.motorSpeed2+'}';
  
  connection.send(full_data);
}

// Receiving data from NODE_MCU
connection.onmessage = function(event){
  var full_data = event.data;
  var data = JSON.parse(full_data); // JSON data into an object
  console.log(data);
  
  if(data.DATA_SOURCE === DataSources.MIXER)
  	updateMixer(data);
    
  else if(data.DATA_SOURCE === DataSources.MODELER)
  	updateModeler(data);
    
  else if(data.DATA_SOURCE === DataSources.GRANULATOR)
  	updateGranulator(data);
    
  else
  	console.log("Data source not found: " + data.DATA_SOURCE);
}

function updateMixer(data) {
  // get data
  mixerData.operating = data.OPERATING;
  mixerData.operationTime = data.OPERATION_TIME / 1000; // milis to seconds
  mixerData.motorSpeed1 = data.MOTOR_SPEED1;

  // set HTML values
  // time
  var timeS = mixerData.operationTime % 60; // remainder of seconds
  var timeM = (mixerData.operationTime - timeS) / 60; // seconds to min
  timeS = Math.trunc(timeS);
  
  document.getElementById("operationTime").innerHTML = timeM + ":" + pad(timeS);
  
  if(operation_time == 0)
  	document.getElementById("txtOperationTime").style.display = ""; // reveal input box
  
  // Speed - SPEED_OFF:0, SPEED_V1:1, SPEED_V2:2
  document.getElementById("sliderAmount").value = mixerData.motorSpeed1 * 50;

  var speedTxt = "";
  if(mixerData.motorSpeed1 == 2)
    speedTxt = "SPEED_V2";
  else if(mixerData.motorSpeed1 == 1)
    speedTxt = "SPEED_V1";
  else if(mixerData.motorSpeed1 == 0)
    speedTxt = "SPEED_OFF";
  else
    speedTxt = mixerData.motorSpeed1;
    
  document.getElementById("motorSpeed").innerHTML = speedTxt;
}

function updateModeler(data) {
  // get data
  modelerData.operating = data.OPERATING;
  modelerData.motorSpeed1 = data.MOTOR_SPEED1; // extruder
  modelerData.motorSpeed2 = data.MOTOR_SPEED2; // molding block
  
  // set HTML values
  document.getElementById("extruderSliderAmount").value = modelerData.motorSpeed1;
  document.getElementById("mBSliderAmount").value = modelerData.motorSpeed2;
}

function updateGranulator(data) {
  // get data
  granulatorData.operating = data.OPERATING;
  granulatorData.motorSpeed1 = data.MOTOR_SPEED1; // rotation blades
  granulatorData.motorSpeed2 = data.MOTOR_SPEED2; // mattres
  
  // set HTML values
  document.getElementById("rotationBladeSliderAmount").value = granulatorData.motorSpeed1;
  document.getElementById("matSliderAmount").value = granulatorData.motorSpeed2;
}

// format seconds display text for mixer
function pad(num) {
    num = num.toString();
    return (num.length < 2) ? num = "0" + num : num;
}

// Mixer Functions -----------------------------------------------------------------
function operationTime(operationTime) {
  var textSpanTime = document.getElementById("operationTime");
  textSpanTime.innerHTML = operationTime + ":00";
  mixerData.operationTime = operationTime * 60; // in seconds
  // operation_time = operationTime * 60; // in seconds
  console.log("Operation Time is set to " + operationTime);
}

function button_1_on()
{ 
  if(mixerData.operationTime === "" || mixerData.operationTime == "0"){
    console.log("Please enter a time, operation time: " + mixerData.operationTime);
    return;
  }
  // hide operation time setter
  document.getElementById("txtOperationTime").style.display = "none";
  
  if(motor_speed == 0)
  {
  	motor_speed = 1;
  	document.getElementById("motorSpeed").innerHTML = "SPEED_V1";
    document.getElementById("sliderAmount").innerHTML = 50;
  }
  
  send_data(mixerData);
}

function button_1_off()
{
  mixerData.operationTime = 0;
  // reveal operation time setter
  document.getElementById("txtOperationTime").style.display = "";
  document.getElementById("operationTime").innerHTML = mixerData.operationTime + ":00";
  
  motor_speed = 0;
  document.getElementById("motorSpeed").innerHTML = "SPEED_OFF";
  document.getElementById("sliderAmount").innerHTML = 0;
  
  send_data(mixerData);
}

function updateSlider(slideAmount) {
  // update mixer motor speed
  mixerData.motorSpeed1 = slideAmount;
  
  // update HTML
  var sliderDiv = document.getElementById("sliderAmount");
  sliderDiv.innerHTML = slideAmount;
  
  var speedTxt = "";
  if(slideAmount > 50)
    speedTxt = "SPEED_V2";
  else if(slideAmount > 0)
    speedTxt = "SPEED_V1";
  else
    speedTxt = "SPEED_OFF";
    
  document.getElementById("motorSpeed").innerHTML = speedTxt;
}


// Modeler Functions -----------------------------------------------------------------
function updateExtruderSlider(slideAmount) {
  // update data
  modelerData.motorSpeed1 = slideAmount;
  
  // update HTML
  var sliderDiv = document.getElementById("extruderSliderAmount");
  sliderDiv.innerHTML = slideAmount;
  
  // update machine data
  send_data(modelerData);
}

function updateMBSlider(slideAmount) {
  // update data
  modelerData.motorSpeed2 = slideAmount;
  
  // update HTML
  var sliderDiv = document.getElementById("mBSliderAmount");
  sliderDiv.innerHTML = slideAmount;
  
  // update machine data
  send_data(modelerData);
}

function buttonExtruderOn() {
  modelerData.operating = true;
  send_data(modelerData);
}

function buttonExtruderOff() {
  modelerData.operating = false;
  send_data(modelerData);
}


// Granulator Functions -----------------------------------------------------------------
function rotationBladeSlider(slideAmount) {
  // update data
  granulatorData.motorSpeed1 = slideAmount;
  
  // update HTML
  var sliderDiv = document.getElementById("rotationBladeSliderAmount");
  sliderDiv.innerHTML = slideAmount;
  
  // update machine data
  send_data(granulatorData);
}

function matSlider(slideAmount) {
  // update data
  granulatorData.motorSpeed2 = slideAmount;
  
  // update HTML
  var sliderDiv = document.getElementById("matSliderAmount");
  sliderDiv.innerHTML = slideAmount;
  
  // update machine data
  send_data(granulatorData);
}

function buttonGranulatorOn() {
  granulatorData.operating = true;
  send_data(granulatorData);
}

function buttonGranulatorOff() {
  granulatorData.operating = false;
  send_data(granulatorData);
}


</script>

<head>
  <title>BRIGADERIA</title>
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
    .card.mixer { color: #B10F2E; }
    .card.extruder { color: #50B8B4; }
    .card.granulator { color: #824dc4; }
  </style>
</head>

<body>
<div class="topnav">
    <h1>Pagina de Controle da Brigaderia</h1>
  </div>
  <div class="content">
    <div class="cards">

      <div class="card mixer">
        <p class="card-title"><i class="fas fa-stopwatch"></i> Tempo de Operacao</p>
        <input type="number" name="txtOperationTime" id="txtOperationTime" placeholder="Enter Time" onchange="operationTime(this.value)">
        <p><span class="reading"><span id="operationTime"></span></span> <i class="fas fa-hourglass"></i> </p>
        <button onclick= "button_1_on()" >On</button>
        <button onclick="button_1_off()" >Off</button>
      </div>
      <div class="card mixer">
        <p class="card-title"><i class="fas fa-tachometer-alt"></i></i> Velocidade Rotacao</p><p>
        <div class="slidecontainer">
          <p>Porcentagem de velocidade de rotacao:</p>
          <input type="range" min="0" max="100" value="0" step="1" onchange="updateSlider(this.value)">
        </div>
        <div id="motorSpeed"></div>
        </p><p class="timestamp">Velocidade: <span id="sliderAmount"></span>%</p>
      </div>
      
      <div class="card extruder">
        <p class="card-title"><i class="fas fa-tachometer-alt"></i> Velocidade Extrusora</p><p>
        <div class="slidecontainer">
          <p>Porcentagem de velocidade de rotacao:</p>
          <input type="range" min="0" max="100" value="0" step="1" onchange="updateExtruderSlider(this.value)">
        </div>
        <button onclick="buttonExtruderOn()" >On</button>
        <button onclick="buttonExtruderOff()" >Off</button>
        <div id="motorSpeed"></div>
        </p><p class="timestamp">Velocidade: <span id="extruderSliderAmount"></span>%</p>
      </div>
      <div class="card extruder">
        <p class="card-title"><i class="fas fa-tachometer-alt"></i> Velocidade Bloco Modelador</p><p>
        <div class="slidecontainer">
          <p>Porcentagem de velocidade de rotacao:</p>
          <input type="range" min="0" max="100" value="0" step="1" onchange="updateMBSlider(this.value)">
        </div>
        <div id="motorSpeed"></div>
        </p><p class="timestamp">Velocidade: <span id="mBSliderAmount"></span>%</p>
      </div>
      
      <div class="card granulator">
        <p class="card-title"><i class="fas fa-tachometer-alt"></i> Velocidade Pas de Rotacao</p><p>
        <div class="slidecontainer">
          <p>Porcentagem de velocidade de rotacao:</p>
          <input type="range" min="0" max="100" value="0" step="1" onchange="rotationBladeSlider(this.value)">
        </div>
        <button onclick="buttonGranulatorOn()" >On</button>
        <button onclick="buttonGranulatorOff()" >Off</button>
        <div id="motorSpeed"></div>
        </p><p class="timestamp">Velocidade: <span id="rotationBladeSliderAmount"></span>%</p>
      </div>
      <div class="card granulator">
        <p class="card-title"><i class="fas fa-tachometer-alt"></i> Velocidade Esteira</p><p>
        <div class="slidecontainer">
          <p>Porcentagem de velocidade de rotacao:</p>
          <input type="range" min="0" max="100" value="0" step="1" onchange="matSlider(this.value)">
        </div>
        <div id="motorSpeed"></div>
        </p><p class="timestamp">Velocidade: <span id="matSliderAmount"></span>%</p>
      </div>

    </div>
  </div>
</body>
</html>
)=====";

const char* Index::getIndexPage()
{
    // return webPage;
    return webPageUpdated;
}