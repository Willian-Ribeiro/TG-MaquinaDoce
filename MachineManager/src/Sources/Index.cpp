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

// send data
var operation_time = 0; //  in seconds
var motor_speed = 0; // SPEED_OFF:0, SPEED_V1:1, SPEED_V2:2

// auxiliar data
var slider_val = 0;

// connection
var connection = new WebSocket('ws://'+location.hostname+':81/');

// When erceiving data from NODE_MCU
connection.onmessage = function(event){
  //console.log("received a JSON message");
  //console.log(event);
  var full_data = event.data;
  //console.log(full_data);
  var data = JSON.parse(full_data); // JSON data into an object
  console.log(data);
  //console.log(data.OPERATION_TIME);
  //console.log(data.MOTOR_SPEED);
  
  // time
  operation_time = data.OPERATION_TIME / 1000; // milis to seconds
  var timeS = operation_time % 60; // remainder of seconds
  var timeM = (operation_time - timeS) / 60; // seconds to min
  timeS = Math.trunc(timeS);
  
  document.getElementById("operationTime").innerHTML = timeM + ":" + pad(timeS);
  
  if(operation_time == 0)
  	document.getElementById("txtOperationTime").style.display = ""; // reveal input box
  
  // Speed
  motor_speed = data.MOTOR_SPEED;
  document.getElementById("sliderAmount").value = motor_speed * 50;

  var speedTxt = "";
  if(motor_speed == 2)
    speedTxt = "SPEED_V2";
  else if(motor_speed == 1)
    speedTxt = "SPEED_V1";
  else if(motor_speed == 0)
    speedTxt = "SPEED_OFF";
  else
    speedTxt = motor_speed;
    
  document.getElementById("motorSpeed").innerHTML = speedTxt;
}

function pad(num) {
    num = num.toString();
    return (num.length < 2) ? num = "0" + num : num;
}

function operationTime(operationTime) {
  var textSpanTime = document.getElementById("operationTime");
  textSpanTime.innerHTML = operationTime + ":00";
  operation_time = operationTime * 60; // in seconds
  console.log("Operation Time is set to " + operationTime);
}

function button_1_on()
{ 
  if(operation_time === "" || operation_time == "0"){
    console.log("Please enter a time, operation time: " + operation_time);
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
  
  send_data();
}

function button_1_off()
{
  operation_time = 0;
  // reveal operation time setter
  document.getElementById("txtOperationTime").style.display = "";
  document.getElementById("operationTime").innerHTML = operation_time + ":00";
  
  motor_speed = 0;
  document.getElementById("motorSpeed").innerHTML = "SPEED_OFF";
  document.getElementById("sliderAmount").innerHTML = 0;
  
  send_data();
}

function updateSlider(slideAmount) {
  var sliderDiv = document.getElementById("sliderAmount");
  sliderDiv.innerHTML = slideAmount;
  slider_val = slideAmount;
  console.log("Slider Amount is" + slideAmount);
  
  var speedTxt = "";
  if(slideAmount > 50)
  {
  	motor_speed = 2;
    speedTxt = "SPEED_V2";
  }
  else if(slideAmount > 0)
  {
  	motor_speed = 1;
    speedTxt = "SPEED_V1";
  }
  else
  {
  	motor_speed = 0;
    speedTxt = "SPEED_OFF";
  }
    
  document.getElementById("motorSpeed").innerHTML = speedTxt;
}

// Sending data to NODE_MCU
function send_data()
{
  var timeMillis = operation_time * 1000;
  var full_data = '{"OPERATION_TIME":'+timeMillis+', "MOTOR_SPEED":'+motor_speed+'}';
  connection.send(full_data);
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
    .card.temperature { color: #B10F2E; }
    .card.humidity { color: #50B8B4; }
  </style>
</head>

<body>
<div class="topnav">
    <h1>Pagina de Controle da Brigaderia</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <p class="card-title"><i class="fas fa-chart-area"></i> Controle de Funcao</p>
        <input type="number" name="txtOperationTime" id="txtOperationTime" placeholder="Enter Time" onchange="operationTime(this.value)">
        <p><span class="reading"><span id="operationTime"></span></span> <i class="fas fa-hourglass"></i> </p>
        <button onclick= "button_1_on()" >On</button>
        <button onclick="button_1_off()" >Off</button>
      </div>
      <div class="card humidity">
        <p class="card-title"><i class="fas fa-ring"></i> Controle de Velocidade</p><p>
        <div class="slidecontainer">
          <p>Motor speed percentage:</p>
          <input type="range" min="0" max="100" value="0" step="1" onchange="updateSlider(this.value)">
        </div>
        <div id="motorSpeed"></div>
        </p><p class="timestamp">Velocidade: <span id="sliderAmount"></span>%</p>
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