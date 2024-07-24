#include "./Headers/Index.h"

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

  constructor(dataSource) {
    this.dataSource = dataSource;
  }
}

const DataSources = {
  MIXER: "mixer",
  MODELER: "modeler",
  GRANULATOR: "granulator",
};

// Mixer motor speed
const MotorSpeed = {
  SPEED_OFF: 0,
  SPEED_V1: 1,
  SPEED_V2: 2,
};

var mixerData = new Data(DataSources.MIXER);
var modelerData = new Data(DataSources.MODELER);
var granulatorData = new Data(DataSources.GRANULATOR);

// auxiliar data
// var slider_val = 0;

// connection
var connection = new WebSocket('ws://'+location.hostname+':81/');

// Sending data to NODE_MCU ---------------------------------------------------------
function send_data(data)
{
  var timeMillis = data.operationTime * 1000;
  
  var full_data = '{"DATA_SOURCE":"'+data.dataSource+'", "OPERATING":'+data.operating+', "OPERATION_TIME":'+timeMillis+', "MOTOR_SPEED1":'+data.motorSpeed1+', "MOTOR_SPEED2":'+data.motorSpeed2+'}';
  
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
  
  if(mixerData.operationTime == 0)
  	document.getElementById("txtOperationTime").style.display = ""; // reveal input box
  
  // Speed - SPEED_OFF:0, SPEED_V1:1, SPEED_V2:2
  document.getElementById("sliderAmount").value = mixerData.motorSpeed1 * 50;

  var speedTxt = "";
  if(mixerData.motorSpeed1 == MotorSpeed.SPEED_V2)
    speedTxt = "SPEED_V2";
  else if(mixerData.motorSpeed1 == MotorSpeed.SPEED_V1)
    speedTxt = "SPEED_V1";
  else if(mixerData.motorSpeed1 == MotorSpeed.SPEED_OFF)
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
  
  mixerData.operating = true;
  if(mixerData.motorSpeed1 == MotorSpeed.SPEED_OFF)
  {
    mixerData.motorSpeed1 = MotorSpeed.SPEED_V1;
  	document.getElementById("motorSpeed").innerHTML = "SPEED_V1";
    document.getElementById("sliderAmount").innerHTML = 50;
  }
  
  send_data(mixerData);
}

function button_1_off()
{
  mixerData.operating = false;
  mixerData.operationTime = 0;
  // reveal operation time setter
  document.getElementById("txtOperationTime").style.display = "";
  document.getElementById("operationTime").innerHTML = mixerData.operationTime + ":00";
  
  mixerData.motorSpeed1 = MotorSpeed.SPEED_OFF;
  document.getElementById("motorSpeed").innerHTML = "SPEED_OFF";
  document.getElementById("sliderAmount").innerHTML = 0;
  
  send_data(mixerData);
}

function updateSlider(slideAmount) {
  // update mixer motor speed and speedText text
  var speedTxt = "";
  if(slideAmount > 50){
  	speedTxt = "SPEED_V2";
  	mixerData.motorSpeed1 = MotorSpeed.SPEED_V2;
  }
  else if(slideAmount > 0){
  	speedTxt = "SPEED_V1";
    mixerData.motorSpeed1 = MotorSpeed.SPEED_V1;
  }
  else{
  	speedTxt = "SPEED_OFF";
    mixerData.motorSpeed1 = MotorSpeed.SPEED_OFF;
  }

  // update HTML
  var sliderDiv = document.getElementById("sliderAmount");
  sliderDiv.innerHTML = slideAmount;
  document.getElementById("motorSpeed").innerHTML = speedTxt;

  send_data(mixerData);
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