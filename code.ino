#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PulseSensorPlayground.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PulseWire = A0;       // Pulse Sensor purple wire connected to analog pin 0
const int LED13 = 13;           // The on-board Arduino LED
int Threshold = 550;            // Heartbeat detection threshold

PulseSensorPlayground pulseSensor;

void setup() {
  lcd.begin(16, 2);             
  lcd.backlight();              
  lcd.setCursor(0, 0);          
  lcd.print("Heart Rate:");
  
  pinMode(LED13, OUTPUT);       

  Serial.begin(9600);           

  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("Sensor Ready");
  }
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();  
  int signal = pulseSensor.getLatestSample();   

  // Normalize the signal to a range of -500 to 500
  int baseline = 512;  // Assuming the baseline (resting value) is around 512
  int normalizedSignal = signal - baseline;

  // Send normalized signal to Serial Plotter
  Serial.println(normalizedSignal);

  if (pulseSensor.sawStartOfBeat()) {           
    digitalWrite(LED13, HIGH);                  
    lcd.setCursor(0, 1);                      
    lcd.print("BPM: ");
    lcd.print(myBPM);                           
    lcd.print("   ");                           
  } else {
    digitalWrite(LED13, LOW);                   
  }

  delay(100);  
}
