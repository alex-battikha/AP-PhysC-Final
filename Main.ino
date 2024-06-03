//By: Alex Battikha

#include <Servo.h> 

boolean running = false;
long previousMillis = 0;        
long interval = 1000;           

Servo myservo1;  
Servo myservo2;  
Servo myservo3;  
Servo myservo4;  
Servo myservo5;  

#define IR_IN  23 

int MenyValg = 0;
int ServoValg = 1;
int ServoValue = 512;
int MenyValgAntall = 15; 
int MenyHigh = 0;

//variables for servo values
int Servo1Value[] = {512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512};
int Servo2Value[] = {512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512}; 
int Servo3Value[] = {512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512}; 
int Servo4Value[] = {512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512}; 
int Servo5Value[] = {512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512}; 

//variables to be updated based on user input
int Servo1ValueNew = 512;
int Servo2ValueNew = 512;
int Servo3ValueNew = 512;
int Servo4ValueNew = 512;
int Servo5ValueNew = 512;

int Servo1ValueOld = 512;
int Servo2ValueOld = 512;
int Servo3ValueOld = 512;
int Servo4ValueOld = 512;
int Servo5ValueOld = 512;

int aminStep = 50;
int a = 0;

void setup() {
  pinMode(IR_IN, INPUT);
  Serial.begin(9600);
  
  //assign servo values to a specific port
  myservo1.attach(3);  
  myservo2.attach(5);  
  myservo3.attach(6);  
  myservo4.attach(9);  
  myservo5.attach(10);  
  
  Serial.println("ARDUINO BOOTING");
}

void loop() {  
  if (running) {
    MenyValg++;
    if (MenyValg > MenyHigh) {
      MenyValg = 0;
    }
    SetServoStepAnim();
  }

  if (Serial.available()) {
    remote_decode();
    remote_deal();
    MenuCase();
  }

  //write servo positions
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
    if (running) {
      digitalWrite(13, HIGH);
      delay(50);
      digitalWrite(13, LOW);
    } 
  }
}

//read the input from the user
void remote_decode() {
  String command = Serial.readStringUntil('\n');
  command.trim(); // Remove any leading or trailing whitespace
  if (command.length() > 0) {
    Serial.println("Received: " + command);
  }
}

void remote_deal() {
  String command = Serial.readStringUntil('\n');
  command.trim(); // Remove any leading or trailing whitespace
  
  if (command.length() > 0) {
    if (command == "start") {
      running = true;
    } else if (command == "stop") {
      running = false;
    } else if (command.startsWith("servo")) {
      int servoNum = command.substring(5, 6).toInt();
      int servoPos = command.substring(7).toInt();
      
      if (servoNum >= 1 && servoNum <= 5) {
        setServo(servoNum, servoPos);
      }
    }
  }
}

void MenuCase() {
}

void setServo(int servoNum, int position) {
  position = constrain(position, 0, 179); // Ensure position is within bounds
  
  //use a switch case to map positions to the servos
  switch (servoNum) {
    case 1:
      myservo1.write(position);
      Servo1ValueNew = map(position, 0, 179, 0, 1023);
      break;
    case 2:
      myservo2.write(position);
      Servo2ValueNew = map(position, 0, 179, 0, 1023);
      break;
    case 3:
      myservo3.write(position);
      Servo3ValueNew = map(position, 0, 179, 0, 1023);
      break;
    case 4:
      myservo4.write(position);
      Servo4ValueNew = map(position, 0, 179, 0, 1023);
      break;
    case 5:
      myservo5.write(position);
      Servo5ValueNew = map(position, 0, 179, 0, 1023);
      break;
  }

  Serial.print("Servo ");
  Serial.print(servoNum);
  Serial.print(" set to ");
  Serial.println(position);
}

void SetServoStepAnim() {
  for (a = 0; a < aminStep; a++) {
    ServoValue = map(a, 0, aminStep, Servo1ValueOld, Servo1Value[MenyValg]);
    ServoValue = map(ServoValue, 0, 1023, 179, 0);
    myservo1.write(ServoValue);
    
    ServoValue = map(a, 0, aminStep, Servo2ValueOld, Servo2Value[MenyValg]);
    ServoValue = map(ServoValue, 0, 1023, 0, 179);
    myservo2.write(ServoValue);
    
    ServoValue = map(a, 0, aminStep, Servo3ValueOld, Servo3Value[MenyValg]);
    ServoValue = map(ServoValue, 0, 1023, 0, 179);
    myservo3.write(ServoValue);
    
    ServoValue = map(a, 0, aminStep, Servo4ValueOld, Servo4Value[MenyValg]);
    ServoValue = map(ServoValue, 0, 1023, 0, 179);
    myservo4.write(ServoValue);
    
    ServoValue = map(a, 0, aminStep, Servo5ValueOld, Servo5Value[MenyValg]);
    ServoValue = map(ServoValue, 0, 1023, 0, 179);
    myservo5.write(ServoValue);
  }
  
  Servo1ValueOld = Servo1Value[MenyValg];
  Servo2ValueOld = Servo2Value[MenyValg];
  Servo3ValueOld = Servo3Value[MenyValg];
  Servo4ValueOld = Servo4Value[MenyValg];
  Servo5ValueOld = Servo5Value[MenyValg];
}

int LoadServoValue() {
  if (MenyValg > MenyHigh) {
    Servo1Value[MenyValg] = Servo1Value[MenyHigh];
    Servo2Value[MenyValg] = Servo2Value[MenyHigh];
    Servo3Value[MenyValg] = Servo3Value[MenyHigh];
    Servo4Value[MenyValg] = Servo4Value[MenyHigh];
    Servo5Value[MenyValg] = Servo5Value[MenyHigh];
    MenyHigh = MenyValg;
  }
  
  switch (ServoValg) {
    case 1:
      ServoValue = Servo1Value[MenyValg];
      break;
    case 2:
      ServoValue = Servo2Value[MenyValg];
      break;
    case 3:
      ServoValue = Servo3Value[MenyValg];
      break;
    case 4:
      ServoValue = Servo4Value[MenyValg];
      break;
    case 5:
      ServoValue = Servo5Value[MenyValg];
      break;
  }
  return ServoValue;
}

//old values of servos saved
void SaveServoValue() {
  switch (ServoValg) {
    case 1:
      Servo1Value[MenyValg] = ServoValue;
      Servo1ValueOld = ServoValue;
      break;
    case 2:
      Servo2Value[MenyValg] = ServoValue;
      Servo2ValueOld = ServoValue;
      break;
    case 3:
      Servo3Value[MenyValg] = ServoValue;
      Servo3ValueOld = ServoValue;
      break;
    case 4:
      Servo4Value[MenyValg] = ServoValue;
      Servo4ValueOld = ServoValue;
      break;
    case 5:
      Servo5Value[MenyValg] = ServoValue;
      Servo5ValueOld = ServoValue;
      break;
  }
}

//write the positions to the servo
void SetServoStepPos() {
  ServoValue = map(Servo1Value[MenyValg], 0, 1023, 179, 0);
  myservo1.write(ServoValue);
  
  ServoValue = map(Servo2Value[MenyValg], 0, 1023, 0, 179);
  myservo2.write(ServoValue);
  
  ServoValue = map(Servo3Value[MenyValg], 0, 1023, 0, 179);
  myservo3.write(ServoValue);
  
  ServoValue = map(Servo4Value[MenyValg], 0, 1023, 0, 179);
  myservo4.write(ServoValue);
  
  ServoValue = map(Servo5Value[MenyValg], 0, 1023, 0, 179);
  myservo5.write(ServoValue);
}
