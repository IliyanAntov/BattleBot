/* 
   -- New project -- 
    
   This source code of graphical user interface  
   has been generated automatically by RemoteXY editor. 
   To compile this code using RemoteXY library 2.3.5 or later version  
   download by link http://remotexy.com/en/library/ 
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                    
     - for ANDROID 4.3.1 or later version; 
     - for iOS 1.3.5 or later version; 
     
   This source code is free software; you can redistribute it and/or 
   modify it under the terms of the GNU Lesser General Public 
   License as published by the Free Software Foundation; either 
   version 2.1 of the License, or (at your option) any later version.     
*/ 

////////////////////////////////////////////// 
//        RemoteXY include library          // 
////////////////////////////////////////////// 

// RemoteXY select connection mode and include library  
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h> 

// RemoteXY connection settings  
#define REMOTEXY_SERIAL Serial 
#define REMOTEXY_SERIAL_SPEED 115200 
#define REMOTEXY_WIFI_SSID "NaStrahilWiFito" 
#define REMOTEXY_WIFI_PASSWORD "battlebotpassword" 
#define REMOTEXY_SERVER_PORT 6377 


// RemoteXY configurate   
#pragma pack(push, 1) 
uint8_t RemoteXY_CONF[] = 
  { 255,5,0,0,0,47,0,8,24,0,
  5,32,6,26,30,30,2,25,31,4,
  48,81,8,12,50,65,31,1,0,39,
  36,19,19,2,31,226,134,146,226,134,
  144,0,1,0,15,12,12,12,2,31,
  51,54,48,0 }; 
   
// this structure defines all the variables of your control interface  
struct { 

    // input variable
  int8_t steering_x; // =-100..100 x-coordinate joystick position 
  int8_t steering_y; // =-100..100 y-coordinate joystick position 
  int8_t acceleration; // =-100..100 slider position 
  uint8_t spin_weapon; // =1 if button pressed, else =0 
  uint8_t rotate; // =1 if button pressed, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY; 
#pragma pack(pop) 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 



#define ENA 5
#define ENB 6

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

#define WEAPONMOTOR1 3
#define WEAPONMOTOR2 4

int spinDirection = 1;
int motorSpeed[2] = {0, 0};
int totalSpeed = 0;

void setup()  
{ 
  RemoteXY_Init ();
  
  pinMode(ENA, OUTPUT);   //Setting up driver pins 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(WEAPONMOTOR1, OUTPUT);
  pinMode(WEAPONMOTOR2, OUTPUT);

  digitalWrite(IN1, HIGH);   //Setting up initial rotation direction
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(ENA, LOW);   //Making sure the motors are stopped
  digitalWrite(ENB, LOW); 

  digitalWrite(WEAPONMOTOR1, LOW); //Making sure the weapon is stopped
  digitalWrite(WEAPONMOTOR2, LOW);

} 

void loop()  
{  
  RemoteXY_Handler ();
  
  if(RemoteXY.rotate){
    Rotate360();
  }
  
  else if(spinDirection != DetermineSpinDirection(RemoteXY.acceleration)){   //Reverse the direction if needed
    spinDirection = !spinDirection;   
    ChangeDirection(spinDirection);
  }
  
  totalSpeed = CalculateTotalSpeed(RemoteXY.acceleration);   //Calculate total motor speed
  CalculateIndividualMotorSpeed(totalSpeed, RemoteXY.steering_x); 

  analogWrite(ENA, motorSpeed[0]);
  analogWrite(ENB, motorSpeed[1]);

  SpinWeapon();


}

int DetermineSpinDirection(int acceleration){  // 0 for backward, 1 for forward
  return (acceleration < 0) ? 0 : 1;
}

int DetermineTurnDirection(int turnModifier){ // 0 for left, 1 for right
  return (turnModifier < 0) ? 0 : 1;
}

void ChangeDirection(int spinDirection){
  if(spinDirection > 0){     //Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else{                      //Backwards   
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
}

int CalculateTotalSpeed(int acceleration){
  return map(abs(acceleration), 0, 100, 0, 255);
}

void CalculateIndividualMotorSpeed(int totalSpeed, int turnModifier){
  int turnDirection = DetermineTurnDirection(turnModifier);
  int modifier = map(abs(turnModifier), 0, 100, 0, 255);
  if(turnDirection == 0){
    motorSpeed[0] = totalSpeed;
    motorSpeed[1] = totalSpeed - modifier;
  }
  else{
    motorSpeed[0] = totalSpeed - modifier;
    motorSpeed[1] = totalSpeed;
  }
}

void Rotate360(){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void SpinWeapon(){
  if(RemoteXY.spin_weapon){
    digitalWrite(WEAPONMOTOR1, HIGH);
    digitalWrite(WEAPONMOTOR2, HIGH);
  }
  else{
    digitalWrite(WEAPONMOTOR1, LOW);
    digitalWrite(WEAPONMOTOR2, LOW);
  }
}
