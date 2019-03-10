/* 
   -- New project -- 
    
   This source code of graphical user interface  
   has been generated automatically by RemoteXY editor. 
   To compile this code using RemoteXY library 2.3.3 or later version  
   download by link http://remotexy.com/en/library/ 
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                    
     - for ANDROID 4.1.1 or later version; 
     - for iOS 1.2.1 or later version; 
     
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


// RemoteXY configuration
#pragma pack(push, 1) 
uint8_t RemoteXY_CONF[] = 
  { 255,6,0,0,0,52,0,8,218,0,
  5,32,6,32,27,27,2,26,31,4,
  48,83,15,11,51,2,26,1,1,37,
  8,27,17,2,31,87,69,80,0,4,
  160,0,0,100,5,2,26,1,0,33,
  48,12,12,2,31,51,54,48,0 }; 
   
// this structure defines all the variables of your control interface  
struct { 

    // input variable
  int8_t left_right_x; // =-100..100 x-coordinate joystick position 
  int8_t left_right_y; // =-100..100 y-coordinate joystick position 
  int8_t acceleration; // =-100..100 slider position 
  uint8_t weapon; // =1 if button pressed, else =0 
  int8_t manual_weapon; // =-100..100 slider position 
  uint8_t spin; // =1 if button pressed, else =0 

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

int spinDirection = 1;

void setup()  
{ 
  RemoteXY_Init ();
  
  pinMode(ENA, OUTPUT);   //Setting up driver pins 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, HIGH);   //Setting up initial rotation direction
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  digitalWrite(ENA, LOW);   //Making sure the motors are stopped
  digitalWrite(ENB, LOW); 

} 

void loop()  
{  
  RemoteXY_Handler ();
  
  if(spinDirection != DetermineSpinDirection(RemoteXY.acceleration)){   //Reverse the direction if needed
    spinDirection = !spinDirection;   
    ReverseDirection(spinDirection);
  }

  int pwmValue = map(abs(RemoteXY.acceleration), 0, 100, 0, 255);   //Calculate motor speed

  analogWrite(ENA, pwmValue); 
  analogWrite(ENB, pwmValue); 
  


  

}

int DetermineSpinDirection(int acceleration){
  return (acceleration < 0) ? 0 : 1;
}

void ReverseDirection(int spinDirection){
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
