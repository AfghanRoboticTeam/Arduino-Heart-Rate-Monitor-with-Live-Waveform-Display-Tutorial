#include <PulseSensorPlayground.h>

const int PulseWire = A0;       // Pulse Sensor purple wire connected to analog pin A0
const int LED13 = 13;           // The on-board Arduino LED
int Threshold = 550;            // Heartbeat detection threshold

PulseSensorPlayground pulseSensor;

void setup() {
  pinMode(LED13, OUTPUT);       

  Serial.begin(9600);           

  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  // Start the sensor
  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor ready");
  }
}

void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();  // Read the BPM
  int signal = pulseSensor.getLatestSample();   // Get the raw sensor data

  // Normalize the signal to a range of -500 to 500
  int baseline = 512;  // Assuming the baseline (resting value) is around 512
  int normalizedSignal = signal - baseline;

  // Send normalized signal to Serial Plotter
  Serial.println(normalizedSignal);

  if (pulseSensor.sawStartOfBeat()) {           // If a heartbeat is detected
    digitalWrite(LED13, HIGH);                  // Blink the LED
    // Optionally print BPM to Serial Monitor (not needed for plot)
    Serial.print("BPM: ");
    Serial.println(myBPM);
  } else {
    digitalWrite(LED13, LOW);                   // Turn off the LED
  }

  delay(100);  // Recommended delay
}
