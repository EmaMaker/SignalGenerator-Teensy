#include <Encoder.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Bounce2.h>

/*CONFIG*/
#define PUSHBUTTON A3
#define ENCODER_STEPS 5
#define TYPE_MIN 0
#define TYPE_MAX 1

#define INTERRUPT_ON TXLED1
#define INTERRUPT_OFF TXLED0

#define DUTY_MIN 10
#define DUTY_MAX 100

#define MAX_ACHIEVABLE_FREQ 1000000
#define FREQ_ARRAY_LEN 7
#define INFO_UPDATE_TIME 2500

// Currently there's a pull up resistors, so it's inverted logic
#define PUSHBUTTON_HIGH LOW
#define PUSHBUTTON_LOW HIGH
/*----------------*/

/*INFO*/
unsigned long frequency = 1000, duty = 50, type = 0;
float Vpp = 6.4, Voff = 0.0;
/*-----------*/

int pushbutton, old_pushbutton;
boolean editingMode = false;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Encoder myEnc(A5, A4);
Bounce pushButton = Bounce();

void setup() {
  Serial1.begin(9600);

  pinMode(PUSHBUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
  
  lcd.init();
  lcd.backlight();

  TXLED0;

  lcd.setCursor(0,0);
  lcd.print("Teensy 3.5 DDS");
  lcd.setCursor(0,1);
  lcd.print("v1.0 by EmaMaker");

  delay(500);

  pushButton.attach(PUSHBUTTON);
  pushButton.interval(25);
  
  communicateChanges();
}

long enc_oldPosition  = -999, enc_newPosition;

void loop() {
  enc_newPosition = myEnc.read();
  pushButton.update();
  
  if(pushButton.fell() && !editingMode) editingMode = true;

  if(editingMode == true) editMode();
  else drawInfoOnScreen();
  
  if (enc_newPosition != enc_oldPosition) {
      enc_oldPosition = enc_newPosition;
  }
}


/*EDIT MODE*/
boolean cleared = false;

long current_digit = 0;
int editing_step = 0;
long* currently_editing;

boolean editing_digit = false;

String s;
long freq_digits[] = {1,0,0,0,0,0,0};

void editMode(){
  if(!cleared){
    lcd.clear();
    cleared = true;

    editing_step = 0;
    current_digit = 0;
    editing_digit = false;

    //Return now and start editing next cycle, otherwise the push button is read twice and type setting is skipped
    return;
  }

  if(editing_step == 0) {
    currently_editing = &type;
    
    lcd.setCursor(0,0);
    lcd.print("Type: ");
    switch(type){
      case 0:
        lcd.print("Sine      ");
        break;
      case 1:
        lcd.print("Square     ");
        break;
    }
  }else if(editing_step == 1) {
    if(type!=1) editing_step++;
    currently_editing = &duty;
    lcd.setCursor(0,0);
    lcd.print("Duty: ");
    lcd.print(duty);
    lcd.print("%     ");
  }else if(editing_step == 2) {
    lcd.setCursor(0,0);
    lcd.print("F: ");

    s = "";
    for(int i = 0; i < FREQ_ARRAY_LEN; i++){
      s += String(freq_digits[i]);
    }
    frequency = s.toInt();
    
    lcd.print(s + "Hz ");
    lcd.setCursor(13, 0);
    lcd.print("Ok?");

    lcd.setCursor(0,1);
    lcd.print("                 "); //clear line

    if(current_digit == FREQ_ARRAY_LEN) lcd.setCursor(15, 1);
    else lcd.setCursor(3+current_digit, 1);

    if(editing_digit) {
      currently_editing = &(freq_digits[current_digit]);
      freq_digits[current_digit] = constrain(freq_digits[current_digit], 0, 9);
      lcd.print("^");
    }else {
      lcd.print("_");
      currently_editing = &current_digit;
    }
    
  }else if(editing_step == 3){
    editingMode = false;
    cleared = false;

    delay(150);

    communicateChanges();
  }

  if (enc_newPosition < enc_oldPosition) (*currently_editing)--;
  else if (enc_newPosition > enc_oldPosition) (*currently_editing)++;

  type = constrain(type, TYPE_MIN, TYPE_MAX);
  duty = constrain(duty, DUTY_MIN, DUTY_MAX);
  current_digit = constrain(current_digit, 0, FREQ_ARRAY_LEN);

  if(pushButton.fell()) {
    if(editing_step != 2 || (editing_step == 2 && current_digit == FREQ_ARRAY_LEN)) editing_step++;
    else editing_digit = !editing_digit;
  }
}

void communicateChanges(){
  //First: toggle the interrupt pin until a positive response is received from teensy
  unsigned long t1 = millis();
  boolean b = false;

  Serial1.flush();
  digitalWrite(LED_BUILTIN, HIGH);
  while(Serial1.read() != 42){  //The answer to Life, Universe, And Everything Else
    if(millis() - t1 > 100){
      b = !b;
      t1 = millis();
      digitalWrite(4, b);
    }
  }
  
  INTERRUPT_OFF;
  t1 = millis();
  /*How the communication protocol works:
  For simplicity, every information is send inside a string
  An information string starts with the character 'w' and ends 'W'
  Every information is added with a special character and terminator, the information is in between
  During construnction of the string, the construction functions are called and add the needed info*/
  
  String info = "w";

  info += "t" + String(type, DEC) + "T";
  info += "f" + String(frequency, DEC) + "F";
  info += "d" + String(duty, DEC) + "D";

  info += "W";
  
  lcd.clear();
  
  //Built the string with the info and set it to the teensy until a positive response is received
  Serial1.flush();
  
  while(Serial1.read() != 69){
    if(millis() - t1 > 250){
  
      lcd.setCursor(0,0);
      lcd.print(info);
      
      Serial1.print(info);
      t1 = millis();
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}


/*-------------*/

/*SHOW MODE*/
boolean b = true;
unsigned long t = 0;

void drawInfoOnScreen(){

  //Alternatevely, type and duty or frequency
  if(millis() - t > INFO_UPDATE_TIME) {
    b = !b;
    t = millis();
    lcd.clear();
  }
  
  //Amplitude, placeholder
  lcd.setCursor(0, 1);
  lcd.print("A:");
  lcd.print(String(Vpp, 1));
  lcd.print("V");

  //Offset
  lcd.setCursor(9, 1);
  lcd.print("O:");
  lcd.print(String(Voff, 1));
  lcd.print("V");
  
  if(b){
    lcd.setCursor(0,0);
    if(type == 0) lcd.print("Sine");
    else if(type == 1) lcd.print("Square");

    //16 - 8 chars for "Duty: __%" + 1 extra char if duty is 100
    lcd.setCursor(8-(duty==100), 0);
    lcd.print("Duty:");
    lcd.print(duty);
    lcd.print("%");
    
  }else{
    lcd.setCursor(0,0);
    lcd.print("Freq: ");
    lcd.print(frequency);
    lcd.print(" Hz");
  }
}
/*-------------*/
