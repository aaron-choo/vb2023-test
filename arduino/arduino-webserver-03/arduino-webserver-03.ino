#include <Arduino_JSON.h>

JSONVar outgoing;

unsigned long timer = 0;

int potVal = 0;
int prevVal = 0;

void setup() {
  outgoing["value"] = 0;
}


void loop() {
  potVal = analogRead(A0);

  if (abs(potVal - prevVal) > 15){
    outgoing["value"] = map(potVal, 0, 1023, 1, 94);
    Serial.println(outgoing);
    prevVal = potVal;
  }

  // potVal = map(analogRead(A0), 0, 1023, 1, 28);

  // if (millis() - timer > 100) {
  //   timer = millis();
  //   outgoing["value"] = potVal;
  //   Serial.println(outgoing);
  // }
}
