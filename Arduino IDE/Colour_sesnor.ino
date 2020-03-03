int x=10;
unsigned int frequency=0;
char c;

void setup() {
  // put your setup code here, to run once:

pinMode(2,OUTPUT); //s0
pinMode(3,OUTPUT); //s1
pinMode(4,OUTPUT); //s2
pinMode(5,OUTPUT); //s3
pinMode(10,INPUT); //vout

digitalWrite(2,HIGH);
digitalWrite(3,LOW);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("Red=");
digitalWrite(4,LOW);
digitalWrite(5,LOW);
frequency = pulseIn(x, LOW);
Serial.print(frequency);
Serial.println();
delay(1500);

Serial.print("Green=");//printing name
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
frequency = pulseIn(x, LOW);
Serial.print(frequency);
Serial.println();
Serial.println("");
delay(1500);


Serial.print("Blue=");//printing name
digitalWrite(4,LOW);
digitalWrite(5,HIGH);
frequency = pulseIn(x, LOW);
Serial.print(frequency);
Serial.println();
delay(1500);

}
