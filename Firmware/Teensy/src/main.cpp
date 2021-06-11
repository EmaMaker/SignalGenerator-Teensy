#include <Arduino.h>
#include "waves.h"

void communicate();
void generate(int, int, int);

bool receivingInfo, valid_flag;
String s, tmp_type, tmp_frequency, tmp_duty;
unsigned long frequency = 1000;
int type = 0, duty = 50;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial1.begin(9600);
  pinMode(21, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(21), communicate, RISING);

  setupWaves();
}

void loop() {
  if(receivingInfo){

    Serial1.write(42);  //The answer to life, universe and everything else
    digitalWriteFast(LED_BUILTIN, HIGH);

    Serial1.flush();

    delay(1000);
    receivingInfo = false;
    valid_flag = false;

    while(!valid_flag){
      while(Serial1.available()) s = Serial1.readStringUntil('W');
      Serial.println(s);
      //Check if it's a valid string, it should start with 'w'
      if(!s.startsWith('w')) continue;

      // Get type
      tmp_type = s.substring(s.indexOf('t')+1, s.indexOf('T'));
      type = tmp_type.toInt();
      // Get duty
      tmp_duty = s.substring(s.indexOf('d')+1, s.indexOf('D'));
      duty = tmp_duty.toInt();
      // Get frequency
      tmp_frequency = s.substring(s.indexOf('f')+1, s.indexOf('F'));
      frequency = tmp_frequency.toInt();

      for(int i = 0; i < 100; i++) Serial1.write(69);
      valid_flag = true;

      valid_flag = true;
      Serial.print("Type: " );
      Serial.println(type);
      Serial.print("Duty: " );
      Serial.println(duty);
      Serial.print("Frequency: " );
      Serial.println(frequency);
      
      digitalWriteFast(LED_BUILTIN, LOW);
      generate(type, frequency, duty);
    }
  }
}

void communicate(){
  receivingInfo = true;
  stopGenerating();
}

void generate(int type, int frequency, int duty){
  switch(type){
    //sine
    case 0:
      generateSine(frequency);
    break;
    case 1:
      generateSquare(frequency, duty);
    break;

  }
}