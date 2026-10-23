#include "AnimationWebServer.h"
#include "Credentials.h"
#include "WebPages.h"
#include "WebScripts.h"
#include "BasicAnimations.h"
#include <map>

//#include <Arduino_LED_Matrix.h>
#define MIN_BRIGHTNESS 0
#define MAX_BRIGHTNESS 255

#ifdef DEBUG
	static const ulong animationMilliSeconds = (ulong)12 * (ulong)1000;// need to cast, otherwise you get int max, idky
#else
	static const ulong animationMilliSeconds = (ulong)90 * (ulong)1000;
#endif

// Marks a parameter as unused, to make the warning go away
#define SUPPRESS_UNUSED_WARNING(x) [&x]{}()

// Marks the web handler parameters as unused
#define SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING()\
	SUPPRESS_UNUSED_WARNING(method);\
	SUPPRESS_UNUSED_WARNING(request);\
	SUPPRESS_UNUSED_WARNING(params);\
	SUPPRESS_UNUSED_WARNING(jsonData);

#define MIME_HTML_TYPE "text/html"
#define MIME_CSS_TYPE "text/css"
#define MIME_JS_TYPE "text/javascript"
#define MIME_JSON_TYPE "application/json"

static UnoR4WiFi_WebServer server;
static const int serverPort = 80;
static bool animationSwitch = false;
// static ArduinoLEDMatrix matrix;
// static const int ledMatrixWidth = 12;
// static const int ledMatrixHeight = 8;
// static bool leds[ledMatrixWidth * ledMatrixHeight];

static ulong startOfCurrentAnimation;
// static const Color plainStrandColor;
static AnimationMode currentMode;
static int currentAnimationIndex;

static bool serverStarted = false;
static bool serverFailed = false;

static std::map<AnimationMode, String> ANIMATION_MODE_TO_STRING;

struct BulbInfo
{
	Color color = Color::White;
};
BulbInfo bulbs[NUM_STRAND_LEDS];

static ColorIterator colorIterator;
static const int NUM_REPEATING_COLORS = 5;
Color repeatingColors[NUM_REPEATING_COLORS];
static int repeatingColorsToUse = 2;
static ColorPalette repeatingPalette(repeatingColors, repeatingColorsToUse);

static byte lightBrightness;

void refreshStrandFromBulbInfo()
{
	for(int bulbIndex = 0; bulbIndex < NUM_STRAND_LEDS; bulbIndex++)
	{
		setLed(bulbIndex, bulbs[bulbIndex].color);
	}

	FastLED.show();
}

void initAnimationModeMap()
{
	ANIMATION_MODE_TO_STRING.clear();

	ANIMATION_MODE_TO_STRING.insert({AnimationMode::Manual, String("Manual")});
	ANIMATION_MODE_TO_STRING.insert({AnimationMode::RepeatingColors, String("RepeatingColors")});
	ANIMATION_MODE_TO_STRING.insert({AnimationMode::RandomAnimation, String("RandomAnimation")});
}

AnimationMode stringToAnimationMode(const String& string)
{
	for (auto it = ANIMATION_MODE_TO_STRING.begin(); it != ANIMATION_MODE_TO_STRING.end(); it++)
	{
		if(string == it->second)
		{
			return it->first;
		}
	}

	return AnimationMode::Manual;
}

void AnimationWebServer::startServer()
{
	server = UnoR4WiFi_WebServer(serverPort);
	startOfCurrentAnimation = millis();
	lightBrightness = 75;
	currentAnimationIndex = 0;

	// Initialize palettes
	colorIterator.setPalette(&repeatingPalette);
	colorIterator.setPairPalette(&COMPLEX_RAINBOW_COLOR_PAIR_PALETTE);
	initAnimationModeMap();

	// Zero out bulbs
	for(int bulbIndex = 0; bulbIndex < NUM_STRAND_LEDS; bulbIndex++)
	{
		bulbs[bulbIndex].color = Color::Red;
	}

	for(int colorIndex = 0; colorIndex < NUM_REPEATING_COLORS; colorIndex++)
	{
		repeatingColors[colorIndex] = Color::White;
	}
	repeatingColors[0] = Color::Red;
	repeatingColors[1] = Color::Green;
	repeatingColors[2] = Color::Blue;
	repeatingColors[3] = Color::Purple;
	repeatingColors[4] = Color::Orange;

	initFastLeds();
	FastLED.setBrightness(lightBrightness);
	FastLED.show();
	
	currentMode = AnimationMode::Manual;
	chooseNextRandomAnimation();

	initServer();
}

void AnimationWebServer::initServer()
{
	if(serverFailed || serverStarted)
		return;

	debugMessage("Arduino Uno R4 WiFi - Web Server");

	// Connect to WiFi
	debugMessage("Creating WIFI access point");
	debugMessage(WIFI_SSID);
	uint8_t started = WiFi.beginAP(WIFI_SSID, WIFI_PASSWORD);
	debugMessage("BeginAP code");

	// Wait forever, printing failure
	if(started == WL_AP_FAILED)
	{
		debugMessage("WIFI failed to start");
		serverFailed = true;
		return;
	}
	
	debugMessage("Wifi started successfully");
	debugMessage("IP address: ");
	debugMessage(WiFi.localIP());

	// Configure routes
	server.addRoute("/", handleHome);

	server.addRoute("/script.js", handleScriptRequest);
	server.addRoute("/style.css", handleStyleRequest);
	server.addRoute("/index.html", handleHome);
	server.addRoute("/points.html", handlePointLeds);

	// Api calls
	server.addRoute("/ledControllerState.json", handleLedControllerStateRequested);
	server.addRoute("/enumerations.json", handleEnumerationsRequested);
	server.addRoute("/callCommand.html", handleCommand);

	// Set custom 404 handler
	server.setNotFoundHandler(handleNotFound);

	// Start server
	server.begin();

	debugMessage("\n=== Web Server Ready! ===");
	debugMessage("Visit: http://");
	debugMessage(WiFi.localIP());
	debugMessage("Waiting for client");
}

ColorIterator& AnimationWebServer::getColorIterator()
{
	return colorIterator;
}

void AnimationWebServer::startAnimationLoop()
{
	// TODO: Put back animations logic
	// if(NUM_ANIMATIONS <= 0)
	// {
	// 	debugMessage("No animations, so exiting animation loop");
	// 	return;
	// }

	debugMessage("Starting animation loop");

	while(true)
	{
		if(!serverFailed)
			server.handleClient();
		animationSwitch = false;
		startOfCurrentAnimation = millis();

		switch(currentMode)
		{
			case AnimationMode::Manual:
				refreshStrandFromBulbInfo();
				debugMessage("Doing manual animation");
				delayUnlessInterrupted(100000);
			break;
			case AnimationMode::RepeatingColors:
				debugMessage("Doing repeating colors animation");
				colorMarch(3);
			break;
			case AnimationMode::RandomAnimation:
			/*
			debugMessage("Doing RandomAnimation ");
			startOfCurrentAnimation = millis();
			ANIMATIONS[currentAnimationIndex]();
			// If we're still in the rotation mode, increment
			if(currentMode == AnimationMode::RandomAnimation)
			chooseNextRandomAnimation();
			*/
			break;
		}
	}
}

void AnimationWebServer::chooseNextRandomAnimation()
{
	// currentAnimationIndex = getNextRandomExclusive(currentAnimationIndex, NUM_ANIMATIONS);
	// toggleLedDirection();
	// debugValue("Newly picked animation index", currentAnimationIndex);
}

bool AnimationWebServer::delayUnlessInterrupted(unsigned int delayMillieseconds)
{
	ulong startTime = millis();
	while(millis() - startTime < delayMillieseconds)
	{
		if(!serverFailed)
			server.handleClient();
		if(animationSwitch)
			return true;
	}

	return false;
}

bool AnimationWebServer::shouldCurrentAnimationContinue()
{
	if(animationSwitch)
		return false;
	return /*currentMode == AnimationMode::HoldAnimation ||*/ millis() - startOfCurrentAnimation < animationMilliSeconds;
}

void AnimationWebServer::setStrandBrightness(int brightness)
{
	lightBrightness = clamp(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();
}

void AnimationWebServer::updateAnimationMode(AnimationMode mode)
{
	if(mode != currentMode)
	{
		debugValue("New animation mode", ANIMATION_MODE_TO_STRING[mode]);
		animationSwitch = true;
		currentMode = mode;
	}
}

void AnimationWebServer::handleHome(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	server.sendResponse(client, HTML_CONTENT_HOME, MIME_HTML_TYPE);
}

void AnimationWebServer::handleScriptRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	server.sendResponse(client, SCRIPTS_JS, MIME_JS_TYPE);
}

void AnimationWebServer::handleStyleRequest(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	server.sendResponse(client, STYLE_CSS, MIME_CSS_TYPE);
}

void AnimationWebServer::handleNotFound(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	server.sendResponse(client, HTML_CONTENT_404, MIME_HTML_TYPE);
}

void AnimationWebServer::handlePointLeds(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	if (method == "GET") 
	{
		server.sendResponse(client, HTML_SET_LEDS_CONTENT, MIME_HTML_TYPE);
	}
}

void AnimationWebServer::handleLedControllerStateRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	const int maxMatrixStringSize =  sizeof("[1,") * NUM_STRAND_LEDS * 5 /*Fudge factor*/;
	char stringBuffer[100]; 

	// Build an array of bulbInfo
	String jsonBuilder = "{\"bulbInfo\":[\n";
	jsonBuilder.reserve(jsonBuilder.length() + maxMatrixStringSize);
	for (int ledIndex = 0; ledIndex < NUM_STRAND_LEDS; ledIndex++) 
	{
		const BulbInfo& info = bulbs[ledIndex];
		sprintf(stringBuffer, "{\"r\": %d, \"g\": %d, \"b\": %d},", info.color.r, info.color.g, info.color.b); 
		jsonBuilder += stringBuffer;
	}
	// Delete trailing comma
	jsonBuilder.remove(jsonBuilder.length() - 1);
	jsonBuilder += "],";

	// Add the repeating colors
	jsonBuilder += "\"repeatingColors\":[\n";
	for(int colorIndex = 0; colorIndex < NUM_REPEATING_COLORS; colorIndex++)
	{
		const Color& color = repeatingColors[colorIndex];
		sprintf(stringBuffer, "{\"r\": %d, \"g\": %d, \"b\": %d},", color.r, color.g, color.b); 
		jsonBuilder += stringBuffer;
	}
	// Delete trailing comma
	jsonBuilder.remove(jsonBuilder.length() - 1);
	jsonBuilder += "],";

	// Add num repeating colors
	sprintf(stringBuffer, "\"repeatingColorsToUse\" : %d,", repeatingColorsToUse); 
	jsonBuilder += stringBuffer;

	// Add animation mode
	sprintf(stringBuffer, "\"animationMode\" : \"%s\",", ANIMATION_MODE_TO_STRING[currentMode].c_str()); 
	jsonBuilder += stringBuffer;

	// Add Brightness
	sprintf(stringBuffer, "\"brightness\" : %d}", lightBrightness); 
	jsonBuilder += stringBuffer;

	server.sendResponse(client, jsonBuilder.c_str(), MIME_JSON_TYPE);
}

void AnimationWebServer::handleEnumerationsRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params,const String& jsonData)
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	char stringBuffer[100]; 
	String jsonBuilder = "{\"animationModes\":[\n";

	for (auto const& x : ANIMATION_MODE_TO_STRING)
	{
		sprintf(stringBuffer, "\"%s\",", ANIMATION_MODE_TO_STRING[x.first].c_str()); 
		jsonBuilder += stringBuffer;
	}

	// Delete trailing comma
	jsonBuilder.remove(jsonBuilder.length() - 1);
	jsonBuilder += "]}";

	server.sendResponse(client, jsonBuilder.c_str(), MIME_JSON_TYPE);
}

void AnimationWebServer::handleCommand(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	debugMessage("Handling " + method + " command: "  + jsonData);
	if(method == "POST")
	{
		int commandIndexEnd = jsonData.indexOf(':');
		String command = jsonData.substring(0, commandIndexEnd);
		String commandParams = jsonData.substring(commandIndexEnd + 1);

		if(command == "SetManualLedColor")
		{
			handleSetManualColorRequested(commandParams);
		}
		else if(command == "SetAllLedsColor")
		{
			handleSetAllLedsColorRequested(commandParams);
		}
		else if(command == "SetRepeatingLedColor")
		{
			handleSetRepeatingLedColor(commandParams);
		}
		else if(command == "SetNumRepeatingColors")
		{
			handleSetNumRepeatingColorsRequested(commandParams);
		}
		else if(command == "SetBrightness")
		{
			handleSetBrightnessRequested(commandParams);
		}
		else if(command == "SetAnimationMode")
		{
			updateAnimationMode(stringToAnimationMode(commandParams));
		}
		else
		{
			debugMessage("UnknownCommand: " + command);
			return;
		}

		server.sendResponse(client, "", MIME_HTML_TYPE);
	}
}

void AnimationWebServer::handleSetManualColorRequested(const String& jsonData) 
{
	updateAnimationMode(AnimationMode::Manual);

	int endOfBulbIndex = jsonData.indexOf(',');
	int endOfRIndex = jsonData.indexOf(',', endOfBulbIndex + 1);
	int endOfGIndex = jsonData.indexOf(',', endOfRIndex + 1);
	String bulbIndexString = jsonData.substring(0, endOfBulbIndex);
	String rValueString = jsonData.substring(endOfBulbIndex + 1, endOfRIndex);
	String gValueString = jsonData.substring(endOfRIndex + 1, endOfGIndex);
	String bValueString = jsonData.substring(endOfGIndex + 1);

	int bulbIndex = bulbIndexString.toInt();
	int r = rValueString.toInt();
	int g = gValueString.toInt();
	int b = bValueString.toInt();
	if(bulbIndex >= 0 && bulbIndex < NUM_STRAND_LEDS)
	{
		bulbs[bulbIndex].color = Color(r, g, b);
	}
	refreshStrandFromBulbInfo();
}

void AnimationWebServer::handleSetAllLedsColorRequested(const String& jsonData) 
{
	updateAnimationMode(AnimationMode::Manual);

	int endOfRIndex = jsonData.indexOf(',');
	int endOfGIndex = jsonData.indexOf(',', endOfRIndex + 1);
	String rValueString = jsonData.substring(0, endOfRIndex);
	String gValueString = jsonData.substring(endOfRIndex + 1, endOfGIndex);
	String bValueString = jsonData.substring(endOfGIndex + 1);
	int r = rValueString.toInt();
	int g = gValueString.toInt();
	int b = bValueString.toInt();

	debugValue("Json recieved", jsonData);
	debugValue("R", r);
	debugValue("G", g);
	debugValue("B", b);
	
	for(int bulbIndex = 0; bulbIndex < NUM_STRAND_LEDS; bulbIndex++)
		bulbs[bulbIndex].color = Color(r, g, b);
	refreshStrandFromBulbInfo();
}

void AnimationWebServer::handleSetRepeatingLedColor(const String& jsonData) 
{
	updateAnimationMode(AnimationMode::RepeatingColors);

	int repeatedColorEndIndex = jsonData.indexOf(',');
	int endOfRIndex = jsonData.indexOf(',', repeatedColorEndIndex + 1);
	int endOfGIndex = jsonData.indexOf(',', endOfRIndex + 1);
	String repeatedColorIndexString = jsonData.substring(0, repeatedColorEndIndex);
	String rValueString = jsonData.substring(repeatedColorEndIndex + 1, endOfRIndex);
	String gValueString = jsonData.substring(endOfRIndex + 1, endOfGIndex);
	String bValueString = jsonData.substring(endOfGIndex + 1);

	int repeatedColorIndex = repeatedColorIndexString.toInt();
	int r = rValueString.toInt();
	int g = gValueString.toInt();
	int b = bValueString.toInt();
	if(repeatedColorIndex >= 0 && repeatedColorIndex < NUM_REPEATING_COLORS)
	{
		repeatingColors[repeatedColorIndex] = Color(r, g, b);
	}
	repeatingPalette.updateColors(repeatingColors, repeatingColorsToUse);
}

void AnimationWebServer::handleSetNumRepeatingColorsRequested(const String& jsonData) 
{
	updateAnimationMode(AnimationMode::RepeatingColors);

	repeatingColorsToUse = jsonData.toInt();
	repeatingPalette.updateColors(repeatingColors, repeatingColorsToUse);
}

void AnimationWebServer::handleSetBrightnessRequested(const String& jsonData) 
{
	int brightness = jsonData.toInt();
	setStrandBrightness(brightness);
}