define DATA 20200319

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // my new standard for blue pill
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Another second.");
  delay(1000);
}
