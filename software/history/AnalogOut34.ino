/* Blink */
// LED_BUILTIN = 5;  // WEMOS LOLIN32 - 115200 baud

int adcPin = 34;
int adcValue = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.print("all good");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  adcValue = analogRead(adcPin);
  Serial.println(adcValue);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(2000); 
}
