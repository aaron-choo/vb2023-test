#include <Servo.h>
#include <Arduino_JSON.h>

JSONVar outgoing;

#define dirPin 2
#define stepPin 3
#define limitPin 7
#define buttonPin 8
#define servoPin 9
#define potPin A0

#define maxStep 8200
#define minStep 0

int currVal = 0;
int prevVal = 0;
int currButtonState = 0;
int prevButtonState = 0;

unsigned long previousMicros = 0;
unsigned long currentMicros;
const long interval = 1000; // Fastest speed (full-step) = 6000

int currentStep;
int plotStep;
int toGo = 0;

boolean initPlotter = false;
boolean triggerPlot = false;
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

  pen.attach(servoPin);
  pen.write(0);

  outgoing["value"] = 0;
  outgoing["activated"] = 0;
  
  findHome();
}

void loop() {


  Serial.println(outgoing);

 currButtonState = buttonState;
 if (currButtonState != prevButtonState) {
      outgoing["activated"] = buttonState;
      prevButtonState = currButtonState;
 }
 
  
  

  if (initPlotter) {
    buttonState = digitalRead(buttonPin);

    

    if (!triggerPlot && buttonState == HIGH) {


      
//      Serial.println(outgoing);

      
      triggerPlot = true;

      int potVal = analogRead(potPin);
      plotStep = map(potVal, 0, 1023, minStep, maxStep);
//      plotStep = int(random(100, maxStep));

      Serial.print("send to: ");
      Serial.println(plotStep);
      moveTo(plotStep, currentStep);

      
      
    }

    // HERE !!
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
      
  }

  currVal = analogRead(A0);

  if (abs(currVal - prevVal) > 15) {
    outgoing["value"] = map(currVal, 0, 1023, 1, 94);
    Serial.println(outgoing);
    prevVal = currVal;
  }
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
  pen.write(180);

  if (currentMillisPen - previousMillisPen >= intervalPen) {
    previousMillisPen = currentMillisPen;
    Serial.println("pen up");
    pen.write(0);
    penDown = false;
    
    currentStep = plotStep;
    triggerPlot = false;
    Serial.println("plot is done");
  }
}

void findHome() {
  Serial.println("finding home");
  plotDirection("up");
  
  while (!initPlotter) {
    Serial.println(digitalRead(limitPin));
    oneStep();
    
    if (digitalRead(limitPin) == HIGH) {
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
