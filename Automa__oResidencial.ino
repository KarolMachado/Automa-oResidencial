#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Servo.h>

char auth[] = "vaSBDC-L4XxKeO59UbyNrlMiEptq9R5Q";
char ssid[] = "Wireless-N";
char pass[] = "65j25m02j01";

#define DHTPIN 15      
#define LEDPINJARDIM 5
#define pinPIR 34
#define ledPIR 26 
#define ledDHT 16
#define DHTTYPE DHT22
#define ledquarto 14
#define ledbanheiro 27

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
Servo servo;
Servo servo1;
int angulo;
int LDRvalue;
uint8_t PWMledPin = 33;
int pwmdata = 0;
float tempmax = 28.5;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha na leitura do sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
 if(t > tempmax){
  digitalWrite(ledDHT,HIGH);}
 else {digitalWrite(ledDHT,LOW);}  
}

void ledJardim() {
  if (LDRvalue < 1000) digitalWrite(LEDPINJARDIM, HIGH);
  else digitalWrite(LEDPINJARDIM, LOW);
}
 
void setup()
{
  ledcAttachPin(PWMledPin, 2);
  ledcSetup(2, 10000, 12);
  pinMode(PWMledPin, OUTPUT);
  pinMode(LEDPINJARDIM, OUTPUT);
  pinMode(pinPIR, INPUT);
  pinMode(ledPIR,OUTPUT);
  pinMode(ledDHT,OUTPUT);
  pinMode(ledquarto,OUTPUT);
  pinMode(ledbanheiro,OUTPUT);
  servo.attach(12);
  servo1.attach(25);
  Blynk.begin(auth, ssid, pass);
  Serial.begin(9600);
  servo.write(0);
  servo1.write(0);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

BLYNK_WRITE(V8) {
  pwmdata = param.asInt();
  ledcWrite(2, pwmdata);
  float voltagepwm = map(pwmdata, 0, 4093, 0, 330);
  Blynk.virtualWrite(V11, voltagepwm / 100);
}

void loop()
{
  Blynk.run();
  timer.run();
  LDRvalue = analogRead(32);
  float Voltagem = map(LDRvalue, 0, 4093, 0, 100); 
  Blynk.virtualWrite(V7, Voltagem);
  ledJardim();
  //WaterLevel();

  bool valorPIR = digitalRead(pinPIR);
  Blynk.virtualWrite(V9,"movimento");
  if(valorPIR){
    Blynk.virtualWrite(V10, "detectado");
    digitalWrite(26,HIGH);}
    else {
    Blynk.virtualWrite(V10, "não-detectado"); 
    digitalWrite(26,LOW);}
}

BLYNK_WRITE(V4) {
  int condicao = param.asInt();
  if(condicao == 1){
  servo1.write(90);}
  else if (condicao == 0){
    servo1.write(0);}
  Blynk.virtualWrite(V13, servo1.read());
}

BLYNK_WRITE(V3) {
  int condicao = param.asInt();
  if(condicao == 1){
  servo.write(90);}
  else if (condicao == 0){
    servo.write(0);}
  Blynk.virtualWrite(V12, servo.read());
}

BLYNK_WRITE(V1){
  int liga1 = param.asInt();
  if(liga1){
    digitalWrite(ledquarto,HIGH);
    }
  else {digitalWrite(ledquarto,LOW);}
  }

 BLYNK_WRITE(V2){
  int liga2 = param.asInt();
  if(liga2){
    digitalWrite(ledbanheiro,HIGH);
    }
  else {digitalWrite(ledbanheiro,LOW);}
  }

