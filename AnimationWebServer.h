#pragma once

#include "Basic.h"
#include "BasicAnimations.h"
#include "FastLED.h"
#include "ColorUtils.h"

#include "AnimationSets.h"
#include "ArduinoGraphics.h"
#include "Credentials.h"
#include "WebPages.h"
#include "UnoR4WiFi_WebServer.h"

enum AnimationMode
{
	PlainColor,
	HoldAnimation,
	RandomAnimation,
};

// A class for controlling animations that the user can interact with via a webserver.
class AnimationWebServer
{
public:
	static void startServer();
	static void startAnimationLoop();

	// Delays the animation, while also waiting for interupts. Returns true if the current animation should be cancelled and returned from
	static bool delayUnlessInterrupted(int delayMillieseconds);
	static bool shouldCurrentAnimationContinue();

private:

	AnimationWebServer() = delete;

	static void chooseNextRandomAnimation();
	static void setStrandColor(Color color);
	static void setStrandBrightness(int brightness);

	static void printToLed(const String text);
	static bool getPixel(int column, int row) ;
	static bool setPixel(int column, int row, bool value) ;

	static void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	/*
	static void handleNotFound(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handleScriptRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handleStyleRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handleLedMessage(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handlePointLeds(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	*/
};