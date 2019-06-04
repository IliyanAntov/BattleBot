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
  { 255,6,0,0,0,70,0,8,24,0,
  5,32,6,27,30,30,30,34,31,4,
  48,78,11,11,43,34,30,1,0,14,
  10,13,13,34,31,51,54,48,0,2,
  0,41,29,20,10,34,26,31,31,87,
  69,80,50,0,79,70,70,0,2,0,
  41,44,20,10,34,26,31,31,87,69,
  80,49,0,79,70,70,0 }; 
   
// this structure defines all the variables of your control interface  
struct { 

    // input variable
  int8_t steering_x; // =-100..100 x-coordinate joystick position 
  int8_t steering_y; // =-100..100 y-coordinate joystick position 
  int8_t acceleration; // =-100..100 slider position 
  uint8_t rotate; // =1 if button pressed, else =0 
  uint8_t spin_weapon_2; // =1 if switch ON and =0 if OFF 
  uint8_t spin_weapon_1; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY; 
#pragma pack(pop) 

///////////////////////////////////////////// 
//           END RemoteXY include          // 
///////////////////////////////////////////// 




#define M1PWM 9
#define M2PWM 10

#define M1DIR 7
#define M2DIR 8

#define WEAPONMOTOR1 2
#define WEAPONMOTOR2 3

#define D2BAR 4

int spinDirection = 1;
int motorSpeed[2] = {0, 0};
int totalSpeed = 0;

void setup()  
{ 
  RemoteXY_Init ();
  
  pinMode(M1PWM, OUTPUT);   //Setting up driver pins 
  pinMode(M1DIR, OUTPUT);
  pinMode(M2PWM, OUTPUT);
  pinMode(M2DIR, OUTPUT);
  pinMode(WEAPONMOTOR1, OUTPUT);
  pinMode(WEAPONMOTOR2, OUTPUT);

  pinMode(D2BAR, OUTPUT);

  digitalWrite(M1DIR, LOW);   //Setting up initial rotation direction
  digitalWrite(M2DIR, LOW);

  digitalWrite(M1PWM, LOW);   //Making sure the motors are stopped
  digitalWrite(M2PWM, LOW); 

  digitalWrite(WEAPONMOTOR1, LOW); //Making sure the weapon is stopped
  digitalWrite(WEAPONMOTOR2, LOW);

  digitalWrite(D2BAR, HIGH); 

} 

void loop()  
{  
  RemoteXY_Handler ();

  if(RemoteXY.connect_flag){
    
    if(spinDirection != DetermineSpinDirection(RemoteXY.acceleration)){   //Reverse the direction if needed
      spinDirection = !spinDirection;   
    }
    
    if(RemoteXY.rotate){  //Checking if the 360 button is pressed
      Rotate360();
    }
    else{
      DetermineDirection(spinDirection);
    }
    
    totalSpeed = CalculateTotalSpeed(RemoteXY.acceleration);   //Calculate total motor speed
    CalculateIndividualMotorSpeed(totalSpeed, RemoteXY.steering_x); 
  
    analogWrite(M1PWM, motorSpeed[0]);
    analogWrite(M2PWM, motorSpeed[1]);
  
    SpinWeapon(); //Checking if any of the weapon buttons are pressed
  }
  
  else{
    digitalWrite(M1PWM, LOW);   //Making sure the motors are stopped
    digitalWrite(M2PWM, LOW); 
  
    digitalWrite(WEAPONMOTOR1, LOW); //Making sure the weapon is stopped
    digitalWrite(WEAPONMOTOR2, LOW);
  }

}

int DetermineSpinDirection(int acceleration){  // 0 for backward, 1 for forward
  return (acceleration < 0) ? 0 : 1;
}

int DetermineTurnDirection(int turnModifier){ // 0 for left, 1 for right
  return (turnModifier < 0) ? 0 : 1;
}

void DetermineDirection(int spinDirection){
  if(spinDirection > 0){     //Forward
    digitalWrite(M1DIR, LOW);
    digitalWrite(M2DIR, LOW);
  }
  else{                      //Backwards   
    digitalWrite(M1DIR, HIGH);
    digitalWrite(M2DIR, HIGH);
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
    digitalWrite(M1DIR, HIGH);
    digitalWrite(M2DIR, LOW);
}

void SpinWeapon(){
  if(RemoteXY.spin_weapon_1){
    digitalWrite(WEAPONMOTOR1, HIGH);
  }
  else{
    digitalWrite(WEAPONMOTOR1, LOW);  
  }
  if(RemoteXY.spin_weapon_2){
    digitalWrite(WEAPONMOTOR2, HIGH);
  }
  else{
    digitalWrite(WEAPONMOTOR2, LOW);
  }
}
