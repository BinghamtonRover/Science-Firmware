// THIS FILE IS FOR TESTING CURRENT SENSOR, IT IS NOT A CLASS YET

void setup()
{
  // put your setup code here, to run once:
  pinMode(23, INPUT);
  analogReadAveraging(4);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println((((3.3 / 1023) * analogRead(23)) - 1.65) / 0.132);
}
