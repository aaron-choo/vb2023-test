#include <Servo.h>
#include <Arduino_JSON.h>

JSONVar outgoing;

#define dirPin 3
#define stepPin 2
#define limitPin 7
#define buttonPin 11
#define servoPin 8
#define potPin A0

#define maxStep 2000
#define minStep 0

int prevVal = 0;
int currButtonState = 0;
int prevButtonState = 0;

unsigned long previousMicros = 0;
unsigned long currentMicros;
const long interval = 2000; // Fastest speed (full-step) = 6000

int currentStep;
int plotStep;
int toGo = 0;

boolean initPlotter = false;
boolean triggerPlot = false;
boolean reset = false;
//boolean plotDone = false;

int buttonState = 0;

Servo pen;
unsigned long currentMillisPen;
unsigned long previousMillisPen = 0;
const long intervalPen = 2000;
boolean penDown = false;

/*
 * =======================================================================
 */

void setup() {
  Serial.begin(9600);
  
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(limitPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pen.attach(servoPin);
  pen.write(90);  // pen off: 90; pen on: 0

  outgoing["value"] = 0;
  outgoing["activated"] = 0;
  
  findHome();
}


void loop() {
   
  if (initPlotter) {

    int potVal;
    buttonState = digitalRead(buttonPin);

    if (!triggerPlot) {
      potVal = analogRead(potPin);

      // Sending value to Aaron's app.js
      if (abs(potVal - prevVal) > 15) {
        outgoing["value"] = map(potVal, 0, 1023, 1, 94);
        Serial.println(outgoing);
        prevVal = potVal;
      }
    }

    if (!triggerPlot && buttonState == LOW) {

      triggerPlot = true;
      plotStep = map(potVal, 0, 1023, minStep, maxStep);

      Serial.print("send to: ");
      Serial.println(plotStep);
      moveTo(plotStep, currentStep);
    }

    if (toGo > 0) {
      oneStep();
    }
  
    if (toGo <= 0 && triggerPlot && !penDown) {
      penDown = true;
      previousMillisPen = millis(); 
    }

    if (penDown) {
      plot();
    }

    if (reset) {
      initPlotter = false;
      findHome();
      reset = false;
    }
  }

// Aaron's code:

//   currVal = analogRead(potPin);

//   if (abs(currVal - prevVal) > 15) {
//     outgoing["value"] = map(currVal, 0, 1023, 1, 94);
//     Serial.println(outgoing);
//     prevVal = currVal;
//   }

//   currButtonState = buttonState;
 
//   if (currButtonState != prevButtonState) {
//       outgoing["activated"] = buttonState;
//       prevButtonState = currButtonState;
//  }

}

/*
 * =======================================================================
 */

void moveTo(int destination, int current) {
  int distance = destination - current;
  
  if (distance > 0) {
    toGo = distance; 
    plotDirection("down"); 
    Serial.println("direction: down");
  }
  
  if (distance < 0) {
    toGo = -distance; 
    plotDirection("up");
    Serial.println("direction: up");
  }
}

void plot() {
  currentMillisPen = millis();

  Serial.println("pen down");
  pen.write(0);

  if (currentMillisPen - previousMillisPen >= intervalPen) {
    previousMillisPen = currentMillisPen;
    Serial.println("pen up");
    pen.write(90);
    penDown = false;
    
    currentStep = plotStep;
    triggerPlot = false;
    Serial.println("plot is done");
    reset = true;
  }
}

void findHome() {
  Serial.println("finding home");
  plotDirection("up");
  
  while (!initPlotter) {
//    Serial.println(digitalRead(limitPin));
    oneStep();
    
    if (digitalRead(limitPin) == LOW) {
      currentStep = minStep;
      
      Serial.println("initialised");
      initPlotter = true;
    }
  }
}

void oneStep() {
  currentMicros = micros();
  
  if (currentMicros - previousMicros >= interval) {
//    Serial.println(toGo);

    previousMicros = currentMicros;
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    toGo -= 1;
  }
}

void plotDirection(String dir) {
  if (dir == "up") digitalWrite(dirPin, HIGH);
  if (dir == "down") digitalWrite(dirPin, LOW);
}
