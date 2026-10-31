#pragma once

#include "Basic.h"
#include "BasicAnimations.h"
#include "FastLED.h"
#include "ColorUtils.h"

#include "AnimationSets.h"
#include "ArduinoGraphics.h"
#include "Credentials.h"
#include "UnoR4WiFi_WebServer.h"
#include "ColorIterator.h"

enum class AnimationMode
{
	Manual,
	RepeatingColors,
	RandomAnimation,
	/* Any new values should be added to the map as well */
};

// A class for controlling animations that the user can interact with via a webserver.
class AnimationWebServer
{
public:
	static void startServer();
	static void startAnimationLoop();

	// Delays the animation, while also waiting for interupts. Returns true if the current animation should be cancelled and returned from
	static bool delayUnlessInterrupted(unsigned int delayMillieseconds);
	static bool pollForInterrupt();
	static bool shouldCurrentAnimationContinue();
	static int getSpeedFactor();
	static long adjustCycleTime(long animationTime);

	static ColorIterator& getColorIterator();

private:

	AnimationWebServer() = delete;

	static void chooseNextRandomAnimation();
	static void setStrandBrightness(int brightness);
	static void initServer();

	static void updateAnimationMode(AnimationMode mode);
	// static bool getPixel(int column, int row) ;
	// static bool setPixel(int column, int row, bool value) ;

	static void handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handleNotFound(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handleScriptRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;
	static void handleStyleRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) ;


	static void handleLedControllerStateRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params,const String& jsonData);
	static void handleEnumerationsRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params,const String& jsonData);
	static void handlePointLeds(WiFiClient& client, const String& method, const String& request, const QueryParams& params,const String& jsonData);

	static void handleCommand(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData); 

	// Commands
	static void handleSetManualColorRequested(const String& jsonData);
	static void handleSetAllLedsColorRequested(const String& jsonData);
	static void handleSetRepeatingLedColor(const String& jsonData);
	static void handleSetNumRepeatingColorsRequested(const String& jsonData);
	static void handleSetBrightnessRequested(const String& jsonData);
	static void handleSetSpeedFactor(const String& jsonData);

};