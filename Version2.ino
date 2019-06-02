 #include <AFMotor.h>

AF_DCMotor motor(2);


const int trigPin=13;
const int echoPin=12;
const int buzzerPin=A1;
const int buttonPin=9;


void setup() {
  // put your setup code here, to run once:
  
  pinMode (trigPin,OUTPUT);
   pinMode (echoPin,INPUT);


  Serial.begin(9600);
  pinMode (buzzerPin,OUTPUT);

  pinMode (buttonPin,INPUT);
  
  analogWrite(buzzerPin,0);
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


float lowpass=0;
float alpha=0.9;
bool InRange=false;

void loop() {
  // put your main code here, to run repeatedly:

   pinMode(buttonPin, OUTPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(buttonPin, INPUT);
  int butread = digitalRead(buttonPin);
  if (butread==LOW)
  {
    
  
   int distance=getDistance(trigPin,echoPin);
   if (distance>0) lowpass=alpha*distance +(1.0-alpha)*lowpass;
    
   Serial.println(lowpass);
 
 
   if ( (lowpass<=80) && (lowpass>=2) )
   {
      InRange=true;
      motor.setSpeed(-3.25*lowpass+261);
     analogWrite(buzzerPin,-3.25*lowpass+200);
    }
    else InRange=false;
  
   if (InRange)
   {
     motor.run(FORWARD);
    // digitalWrite(buzzerPin,HIGH);
   }
   else 
   {
     motor.run(RELEASE);
     analogWrite(buzzerPin,0);
   }
  }
  else
  {
    motor.setSpeed(0);
    analogWrite(buzzerPin,0);
  }
} 
