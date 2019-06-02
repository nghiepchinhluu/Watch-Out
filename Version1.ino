#include <Wire.h>
#include "rgb_lcd.h"
#include <math.h>

rgb_lcd lcd;
const int trigPin1 = 9;
const int echoPin1 = 10;
const int trigPin2 = 11;
const int echoPin2 = 12;


namespace UltrasonicSensor
{
  bool checkcondition(int value)
  {
    if (value>120) return false;
    if (value<0) return false;
    return true;
  }

  int getDistance(int trigPin, int echoPin)
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds (10);
    digitalWrite (trigPin,LOW);
    delayMicroseconds (2);
    int duration = pulseIn (echoPin,HIGH);
    int distance=duration*0.034/2;
    
    return distance;
  }

  int VectorCombination(int vectorA, int vectorB)
  {
   int resultVector=(int)sqrt(vectorA*vectorA+vectorB*vectorB);
   return resultVector;
  }

  int DegreefromVectorA(int vectorA, int vectorB) //online version
  {
    // 1: fall to A(left), 2: fall to B(right), 0: middle,
   double ratio= (double)vectorB/(double)vectorA;
   if (ratio>2)return 1;
   if (ratio<0.5) return 2;
   return 0;
  } 
}

namespace DrawLCD
{
  
  void RightSide(int sta,int en)
  {
   lcd.setCursor(sta,0);
   for (int i=sta;i<=en;i++)
   lcd.print((char)255);
   lcd.setCursor(sta,1);
   for (int i=sta;i<=en;i++)
   lcd.print((char)255);
  }

  void LeftSide(int sta,int en)
  {  
   for (int i=sta;i>=en;i--)
   {
     lcd.setCursor(i,0);
     lcd.print((char)255);
   }
   for (int i=sta;i>=en;i--)
   {
     lcd.setCursor(i,1);
     lcd.print((char)255);
   }
  }

  void Middle()
  {
   for (int le=0;le<=4;le++)
   {
     lcd.clear();
     LeftSide(7,7-le);
     RightSide(8,8+le);
     delay(100);
   }
  }
}

void setup() 
{
  // Setup lcd blacklight
  lcd.begin(16,2);
  lcd.setRGB(130,130,130);
  lcd.clear();
  lcd.println("COLLISION DETECT");
  
  delay(1000);

  // Setup input output for ultrasonic sensors
  pinMode (trigPin1,OUTPUT);
  pinMode (echoPin1,INPUT);
  pinMode (trigPin2, OUTPUT);
  pinMode (echoPin2, INPUT);
  //Serial.begin(9600);
}

float lowpass=0.0;
float alpha=0.9;
int red,blue,green;

void loop() 
{
  //first sonar 
  
    lcd.clear();
    int distance1=UltrasonicSensor::getDistance(trigPin1,echoPin1);
    delayMicroseconds (500);
    int distance2=UltrasonicSensor::getDistance(trigPin2,echoPin2);
  
  /*  lowpass=alpha*distance1 +(1.0-alpha)*lowpass;
    distance1=(int)lowpass;
    
    lowpass=alpha*distance2 +(1.0-alpha)*lowpass;
    distance2=(int)lowpass;
    */

  if ( (distance1<0) || (distance2<0) ) 
  {
    delay(500);
    return;
  }
  if ((distance1>120) && (distance2>120)) // Safe distance
  {
    lcd.setRGB(0,250,0);
    delay(500);
    return;
  }
  
  lcd.setRGB(240,30,0);

  //Serial.print("Distance: ");
  //Serial.print(distance1);

  //Serial.print("    Distance 2: ");
  //Serial.print(distance2);

  int com_distance=UltrasonicSensor::VectorCombination(distance1,distance2);
   
  int degreefromvectA=UltrasonicSensor::DegreefromVectorA(distance1,distance2);
 // Serial.print("    Combination distance: ");
 // Serial.print(com_distance);

 // Serial.print("    Angle: ");
  int tmp=degreefromvectA;
  if (tmp==1) 
  {
 //   Serial.println("Left");
    for (int k=5;k>=0;k--)
    {
      lcd.clear();
      DrawLCD::LeftSide(5,k);
      delay(100);
    }
  }
  else if (tmp==2) 
  {
 //   Serial.println("Right");
    for (int k=10;k<=15;k++)
    {
      lcd.clear();
      DrawLCD::RightSide(10,k);
      delay(100);
    }
  }
  else 
  {
 //   Serial.println("Middle");
    DrawLCD::Middle();
  }
  delay(2000);
}
