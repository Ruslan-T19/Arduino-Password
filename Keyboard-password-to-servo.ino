#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);  //
Servo servo1;

const byte ROWS = 4;
const byte COLS = 4;

#define Password_Length 12

int signalPin = 12;

char Data[Password_Length];
char Master[Password_Length] = "4444";
byte data_count = 0;
bool Pass_is_good = false;
boolean p = false;
char customKey;
const int button = 12;
boolean btt;
boolean butt_flag = 0;
unsigned long last_press;
boolean flag_open = false;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

byte rowPins[ROWS] = {10,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  // put your setup code here, to run once:
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
   servo1.attach(11);
   lcd.init();                      // Инициализация дисплея  
   lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
   int count = 0;
   if(count == 1){
        lcd.clear();
   }

   lcd.setCursor(0,0);
   lcd.print("Enter password:*");
   lcd.setCursor(0,1);
   lcd.print("Change password:#");
   char customKey = customKeypad.getKey();

   switch(customKey){
     case '*': 
     
      while(p == false && flag_open == false){
         enterPassword();
         delay(1500);
      }
      p = false;
      Serial.println("Privet");
      Serial.println(Data);
      Serial.println(Master);
     
     count = 1;

     break;
     case '#':
      
      if(Pass_is_good){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("New password");
     
      memset(Master, 0, Password_Length * (sizeof Master[0]) );
      data_count = 0;
        while(data_count < 4){
            customKey = customKeypad.getKey();
            
          if (customKey){
              Master[data_count] = customKey;
              lcd.setCursor(data_count,1);
              lcd.println(Master[data_count]);
              data_count++;
            }
        }
      }else{
         enterPassword();
         break;
      }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("You new password");
        lcd.setCursor(0, 1);
        lcd.print(Master);
        delay(1500);
        p = false;
      count = 1;
     break;
   }
   btt = !digitalRead(button);
   if (btt == 1 && butt_flag == 0 && millis() - last_press > 100){//Закрытие
       if(Pass_is_good){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Closed");
          delay(600);
          servo1.write(90);
          delay(1500);
          butt_flag = 1;
          flag_open = false;
        last_press = millis();
       }
       Pass_is_good = false;
     }
     if(btt == 0 && butt_flag == 1){
       butt_flag = 0;
     }
}
void enterPassword(){
   lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password");
      data_count = 0;
  while(data_count < 4){//Ввод пароля
          customKey = customKeypad.getKey();
        if (customKey){
            Data[data_count] = customKey;
            lcd.setCursor(data_count,1);
            lcd.print(Data[data_count]);
            data_count++;
          }
      }
  data_count = 0;
  p = true;
   if(!strcmp(Data, Master)){ //Проверка пароля
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Open");
        servo1.write(180);
        delay(1500);
        flag_open = true;
        Pass_is_good = true;
      }else{
        lcd.setCursor(0,0);
        lcd.print("wrong password");
        delay(1500);
        Pass_is_good = false;
      }
   delay(500);
}
