/**
 * Machine Manager Firmware
 * Copyright (c) 2020 MachineManagerFirmware [https://github.com/Willian-Ribeiro/TG-MaquinaDoce.git]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */


// hw_timer_t *timer = null;

// timer = timerBegin(0, 80, true);

// timerAttachInterrupt(timer, &resetModule, true);

// timerAlarmWrite(timer, 3000000, true);

// timerAlarmEnable(timer); //habilita a interrupção

// timerWrite(timer, 0);


/*
#include <Arduino.h>


const int pinButton = 2; //pino do botão para simular um travamento
const int redLed = 23; //pino do led
const int greenLed = 4; //esse led acenderá toda vez que o ESP32 reiniciar

hw_timer_t *timer = NULL; //faz o controle do temporizador (interrupção 


// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(115200);
    Serial.println("running setup");
    
    pinMode(pinButton, INPUT_PULLUP);
    pinMode(redLed , OUTPUT);
    pinMode(greenLed , OUTPUT);   
    delay(500);  

    //hw_timer_t * timerBegin(uint8_t num, uint16_t divider, bool countUp)
    /*
       num: é a ordem do temporizador. Podemos ter quatro temporizadores, então a ordem pode ser [0,1,2,3].
      divider: É um prescaler (reduz a frequencia por fator). Para fazer um agendador de um segundo, 
      usaremos o divider como 80 (clock principal do ESP32 é 80MHz). Cada instante será T = 1/(80) = 1us
      countUp: True o contador será progressivo
    * /
    timer = timerBegin(0, 80, true); //timerID 0, div 80
    //timer, callback, interrupção de borda
    timerAttachInterrupt(timer, &resetModule, true);
    //timer, tempo (us), repetição
    timerAlarmWrite(timer, 3000000, true);
    timerAlarmEnable(timer); //habilita a interrupção 
    digitalWrite(greenLed, HIGH);
    delay(1000);
    digitalWrite(greenLed, LOW);
}

void loop() {
    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog) 
    long tme = millis(); //tempo inicial do loop
    //fica preso no loop enquanto o botão estiver pressionado
    while(digitalRead(pinButton))
    {
        Serial.println("botão pressionado: "+String(millis() - tme));
        digitalWrite(redLed, HIGH); //acende o led vermelho
        delay(500);
//        timerWrite(timer, 0); //se fizer esse reset no temporizador, o watchdog não será acionado
    }
    delay(1000);
    Serial.print("tempo passado dentro do loop (ms) = ");
    tme = millis() - tme; //calcula o tempo (atual - inicial)
    Serial.println(tme);
    //desliga o led vermelho
    digitalWrite(redLed, LOW);
}

//função que o temporizador irá chamar, para reiniciar o ESP32
void IRAM_ATTR resetModule(){
    ets_printf("(watchdog) reiniciar\n"); //imprime no log
    esp_restart_noos(); //reinicia o chip
}

*/



