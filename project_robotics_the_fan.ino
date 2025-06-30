#define PIEZO 8
int baselineTemp = 30;
int celsius = 0;
int LED = 9;
const int BUTTON = 2;  // BUTTON is a constant
int sensorValue;            // Stores the value of the sensor (0-1023)
unsigned long startTime = 0; // Variable to store when the fan started
int melody[] = {262, 196, 196, 220, 196, 0, 247, 262}; // 8 music notes
int noteDurations[] = {250, 125, 125, 250, 250, 250, 250, 250};
boolean on = false;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);  // Optional. TMP sensor
  pinMode(A1, INPUT);  // Optional. Photoresistor
  pinMode(PIEZO, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT); // input for the button
}

void loop() {
  if (digitalRead(BUTTON) == HIGH){
    if (!on) // if (on == false)
       on = true;
    else
       on = false;  
  }
  if(on){
  // Measure temperature in Celsius
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);
  Serial.print(celsius);
  Serial.println(" C");

  if (celsius > baselineTemp) {
    // If the fan just started, record the start time
    if (startTime == 0) {
      startTime = millis();
      Serial.println(startTime);
    }

    // Check if 5 minutes (300,000 ms) have passed
    if (millis() - startTime >= 3000) { 
      // Play the melody
      for (int i = 0; i < 8; i++) {
        tone(PIEZO, melody[i], noteDurations[i]); // Play a note
        delay(100);
        noTone(PIEZO); // Stop the tone playing
      }
      
      // Reset the timer to measure the next 5-minute interval
      startTime = millis();
    }

    // Read the value from the sensor (range [0…1023])
    sensorValue = analogRead(A1);

    // Print the sensor reading
    Serial.print(sensorValue);
    Serial.println(" Brightness");

    // Map the sensor reading to the range [0…255] to set the LED brightness
    analogWrite(LED, map(sensorValue, 0, 1023, 0, 255));
    delay(100); // Wait for 100 milliseconds
  } else {
    // Reset variables if the fan is off
    startTime = 0;
    digitalWrite(LED, LOW);
  }
  }else{
    digitalWrite(LED, LOW);
  }
  delay(10);
}
