/* Blink */
// LED_BUILTIN = 5;  // WEMOS LOLIN32 - 115200 baud

int adcPin = 34;
int batPin = 35;
int adcValue = 0;
int batValue = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Lets start!");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  adcValue = analogRead(adcPin);
  Serial.print("34 has ");
  Serial.print(adcValue);
  batValue = analogRead(batPin);  
  Serial.print(" and battery value is ");
  Serial.println(batValue);
  Serial.print("Voltage is: ");
  Serial.print(int(adcValue * 0.826 + 150));
  Serial.print(" and Battery voltage is ");
  Serial.println(int((batValue * 0.826 + 150)*2));
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(2000); 
}
