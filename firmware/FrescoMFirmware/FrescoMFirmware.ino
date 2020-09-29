#include "Pins.h" 
#include "EndStopper.h"
#include "Command.h"
#include "EndStopper.h"
#include "FrescoMotor.h"
#include "FrescoXYZ.h"
#include "MotorController.h"
#include "Parser.h"

int stepPins[] = {XS1,YS1,ZS1,AS1, XS2, YS2, ZS2, AS2, XS3, YS3, ZS3, AS3};
int dirPins[] = {XD1, YD1, ZD1, AD1, XD2, YD2, ZD2, AD2, XD3, YD3, ZD3, AD3};
int endStopperPins[] = {ES1, ES2, ES3, ES4, ES5, ES6, ES7, ES8, ES9, ES10, ES11, ES12};

void setupPinsModeEndSetEnabled() {
  
  for (int i = 0; i < NUM_MOTORS; i++){
    pinMode(stepPins[i], OUTPUT);
    pinMode(dirPins[i], OUTPUT);
    pinMode(endStopperPins[i], INPUT);
  }
  
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);
  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);

  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
}

void setupSerial() {
  Serial.begin (250000); 
  Serial.setTimeout(25);
}

FrescoXYZ* frescoXYZ;
Parser* frescoParser;

void setup() {
  setupSerial();

  setupPinsModeEndSetEnabled();
  
  FrescoMotor* xMotor = new FrescoMotor(XS1, XD1);
  FrescoMotor* yMotor = new FrescoMotor(YS1, YD1);
  FrescoMotor* zMotor = new FrescoMotor(ZS1, ZD1);

  EndStopper* xStopper = new EndStopper(ES1);
  EndStopper* yStopper = new EndStopper(ES2);
  EndStopper* zStopper = new EndStopper(ES3);

  MotorController* xMotorController = new MotorController(xMotor, xStopper);
  MotorController* yMotorController = new MotorController(yMotor, yStopper);
  MotorController* zMotorController = new MotorController(zMotor, zStopper);

  frescoXYZ = new FrescoXYZ(xMotorController, yMotorController, zMotorController);
  frescoParser = new Parser();
}

void loop() {
  if (Serial.available() > 0) {
    String line = Serial.readString();
    Command command = frescoParser->parse(line);
    frescoXYZ->perform(command);
  }
}