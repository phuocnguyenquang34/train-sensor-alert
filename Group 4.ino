#include <Servo.h>
Servo barie;

#define led1 11
#define led2 12

#define but3 2
#define but2 3
#define but1 4

#define loa 13

#define ping1 A0
#define ping2 A1
#define ping3 A2
#define ping4 A3
#define ping5 A4
#define ping6 A5

#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
// include the library code:
#include <LiquidCrystal.h>
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(10, 9, 8, 7, 6, 5);
int bienphu;
void setup() {
  Serial.begin(9600);
  //Cai dat LCD
  lcd.begin(16, 2);
  //Cai dat LED
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  //Cai dat loa
  pinMode(loa, OUTPUT);
  //Cai dat nut
  pinMode(but1, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(but1), testServo, FALLING);
  pinMode(but2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(but2), testSpeaker, FALLING);
  pinMode(but3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(but3), testLed, FALLING);
  //Servo
  barie.attach(11);
}

void loop() {
  //Quet nut
  int pressBut1 = digitalRead(but1);
  if (pressBut1==0) bienphu=1;
  if (pressBut1==1 && bienphu==1) {testServo();bienphu=0;}
  
  lcd.setCursor(0, 0);
  lcd.print("HE THONG SANSANG");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  barie.write(0);
  
  if(!senseTrainPing(ping1)&&senseTrainPing(ping6)){
      tauDiXuoi();
    }
  if(!senseTrainPing(ping6)&&senseTrainPing(ping1)){
      tauDiNguoc();
    }
}

void tauDiXuoi(){
  while(!senseTrainPing(ping1)){
  lcd.setCursor(0, 0);
  lcd.print("HE THONG SANSANG");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  barie.write(0);
    
  }
  while(senseTrainPing(ping1)){
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("TAU SAP DEN1");
  lcd.setCursor(0, 1);
  lcd.print("-HA BARIE");
  barie.write(90);
    
  }
  while(!senseTrainPing(ping1)&&senseTrainPing(ping2)){
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
  	lcd.print("TAU DEN GAN2");
    lcd.setCursor(0, 1);
  	lcd.print("            ");
    //Den nhay
    digitalWrite(led1, 1);delay(100);
    digitalWrite(led1, 0);digitalWrite(led2, 1);delay(100);
    digitalWrite(led2, 0);
    
  }
  while(!senseTrainPing(ping1)&&!senseTrainPing(ping2)&&(senseTrainPing(ping3)||senseTrainPing(ping4)||senseTrainPing(ping5))){
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
  	lcd.print("TAU DANG DI QUA");
    //Den nhay
    digitalWrite(led1, 1);delay(100);
    digitalWrite(led1, 0);digitalWrite(led2, 1);delay(100);
    digitalWrite(led2, 0);
    //Coi keu
    //digitalWrite(loa, 1); 
    coiKeu();
    
  }
  while(senseTrainPing(ping6)){barie.write(0);digitalWrite(loa, 0);}
}

void tauDiNguoc(){
  while(!senseTrainPing(ping6)){
  lcd.setCursor(0, 0);
  lcd.print("HE THONG SANSANG");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  barie.write(0);
    
  
  }
  while(senseTrainPing(ping6)){
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("TAU SAP DEN6");
  lcd.setCursor(0, 1);
  lcd.print("-HA BARIE");
  barie.write(90);
    
    
  }
  while(!senseTrainPing(ping6)&&senseTrainPing(ping5)){
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
  	lcd.print("TAU DEN GAN5");
    lcd.setCursor(0, 1);
  	lcd.print("            ");
    //Den nhay
    digitalWrite(led1, 1);delay(100);
    digitalWrite(led1, 0);digitalWrite(led2, 1);delay(100);
    digitalWrite(led2, 0);
    
    
  }
  while(!senseTrainPing(ping6)&&!senseTrainPing(ping5)&&(senseTrainPing(ping4)||senseTrainPing(ping3)||senseTrainPing(ping2))){
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
  	lcd.print("TAU DANG DI QUA");
    //Den nhay
    digitalWrite(led1, 1);delay(100);
    digitalWrite(led1, 0);digitalWrite(led2, 1);delay(100);
    digitalWrite(led2, 0);
    //Coi keu
    coiKeu(); 
    
  }
  while(senseTrainPing(ping1)){barie.write(0);digitalWrite(loa, 0);}
}

void coiKeu(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(loa, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(loa);
  }
}

void senseDistance(int pingPin){
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  Serial.print(duration / 29 / 2);Serial.println("cm");

  delay(50);
}

bool senseTrainPing(int pingPin){
  long duration;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  if(duration < 19400) return true;
  else return false;
}


void testServo(){
  lcd.setCursor(0,0);lcd.print("                ");
  int pos;
  lcd.setCursor(0,0);lcd.print("TEST BARIE SERVO");
  
  for(pos = 0; pos <= 180; pos++) 
  {                                  
    barie.write(pos);      
    delay(15);                    
  } 
  for(pos = 180; pos>=0; pos--)     
  {                                
    barie.write(pos);              
    delay(15);                     
  } 
}

void testSpeaker(){
  lcd.setCursor(0,0);lcd.print("                ");
  lcd.setCursor(0,0);lcd.print("TEST LOA");
  for(int i=0;i<50;i++)
  {
    digitalWrite(loa,1);delay(100);
  }
  digitalWrite(loa,0);
  
}

void testLed(){
  lcd.setCursor(0,0);lcd.print("                ");
  int pos;
  lcd.setCursor(0,0);lcd.print("TEST LED");
  for(int i = 0; i < 50; i++){
  	digitalWrite(led1, 1);delay(100);
    digitalWrite(led1, 0);digitalWrite(led2, 1);delay(100);
    digitalWrite(led2, 0);
  }
}
 
