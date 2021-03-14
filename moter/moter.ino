void setup() {
  pinMode(D0, OUTPUT); 
  pinMode(D1, OUTPUT); 
  pinMode(D4, OUTPUT); 

}


void runMoter1(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(D0, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                     
    digitalWrite(D0, LOW);
    delay(500);                     
  }
}


void runMoter2(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(D1, HIGH);   
    delay(500);                     
    digitalWrite(D1, LOW);
    delay(500);                     
  }
}


void runMoter3(int n)
{
  for(int i=0; i<n; i++)
  {
    digitalWrite(D4, HIGH);   
    delay(500);                     
    digitalWrite(D4, LOW);
    delay(500);                     
  }
}


void loop() {

    runMoter1(int(3.0));
    runMoter2(int(2.0));
    runMoter3(int(1.0));


    delay(4000);
      
}
