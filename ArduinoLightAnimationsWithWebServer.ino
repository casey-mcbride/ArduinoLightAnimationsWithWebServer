// #include "LedClusterAnimations.h"
#include "RemoteAnimationController.h"
#include "Configuration.h"
#include "Basic.h"
// #include "ColorUtils.h"
// #include "IRremote.hpp"
// #include "BasicAnimations.h"
// #include "ColorUtils.h"

// typedef void* (int) AnimationFunction;

// #define LED_DATA_PIN 6
#define MAX_BRIGHTNESS 100
// typedef void* (int) AnimationFunction;

void setup()
{
	initDebugIO();

	// If nothing in 0, noise produces a semi-random number
	randomSeed(analogRead(0));
	RemoteAnimationController::startRecieving(IR_RECIEVER_DATA_PIN);
}

void loop()
{
	RemoteAnimationController::startAnimationLoop();
	// RemoteAnimationController::testIRReceiverLoop();
}