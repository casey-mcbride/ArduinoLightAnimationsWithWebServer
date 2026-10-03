#include "AnimationWebServer.h"
#include "Credentials.h"
#include "WebPages.h"
#include "WebScripts.h"

//#include <Arduino_LED_Matrix.h>
#define MIN_BRIGHTNESS 5
#define MAX_BRIGHTNESS 100

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
#define MIME_JASON_TYPE "application/json"

static UnoR4WiFi_WebServer server;
static const int serverPort = 80;
// static ArduinoLEDMatrix matrix;
// static const int ledMatrixWidth = 12;
// static const int ledMatrixHeight = 8;
// static bool leds[ledMatrixWidth * ledMatrixHeight];

static ulong startOfCurrentAnimation;
// static const Color plainStrandColor;
static AnimationMode currentMode;
static int currentAnimationIndex;

struct BulbInfo
{
	Color color = Color::White;
};
BulbInfo bulbs[NUM_STRAND_LEDS];

static const int NUM_REPEATING_COLORS = 5;
Color repeatingColors[NUM_REPEATING_COLORS];
int repeatingColorsToUse = 2;

static int lightBrightness;

void refreshStrandFromBulbInfo()
{
	for(int bulbIndex = 0; bulbIndex < NUM_STRAND_LEDS; bulbIndex++)
	{
		setLed(bulbIndex, bulbs[bulbIndex].color);
	}

	FastLED.show();
}

void AnimationWebServer::startServer()
{
	server = UnoR4WiFi_WebServer(serverPort);
	startOfCurrentAnimation = millis();
	lightBrightness = 75;
	currentAnimationIndex = 0;

	// Zero out bulbs
	for(int bulbIndex = 0; bulbIndex < NUM_STRAND_LEDS; bulbIndex++)
	{
		bulbs[bulbIndex].color = Color::White;
	}

	for(int colorIndex = 0; colorIndex < NUM_REPEATING_COLORS; colorIndex++)
	{
		repeatingColors[colorIndex] = Color::White;
	}

	Serial.println("Arduino Uno R4 WiFi - Web Server");

	// Connect to WiFi
	Serial.print("Creating WIFI access point");
	Serial.println(WIFI_SSID);
	uint8_t started = WiFi.beginAP(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("BeginAP code");

	// Wait forever, printing failure
	while(started == WL_AP_FAILED)
	{
		printToLed("WIFI failed to start");
	}
	
	printToLed("Wifi started successfully");

	Serial.println("Waiting for client connection");
	while (WiFi.status() != WL_AP_CONNECTED)
	{
		delay(500);
		printToLed(WiFi.localIP().toString());
	}

	Serial.println("connected!");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	// Configure routes
	server.addRoute("/", handleHome);

	server.addRoute("/script.js", handleScriptRequest);
	server.addRoute("/style.css", handleStyleRequest);
	server.addRoute("/index.html", handleHome);
	server.addRoute("/ledmessage.html", handleLedMessage);
	server.addRoute("/points.html", handlePointLeds);

	// Api calls
	server.addRoute("/ledControllerState.json", handleLedControllerStateRequested);
	server.addRoute("/setManualLedColor.html", handleSetManualColorRequested);
	server.addRoute("/setAllLedsColor.html", handleSetAllLedsColorRequested);
	server.addRoute("/setRepeatedColor.html", handleSetRepeatedColorRequested);
	server.addRoute("/setNumRepeatedColors.html", handleSetNumRepeatedColorsRequested);

	// Set custom 404 handler
	server.setNotFoundHandler(handleNotFound);

	// Start server
	server.begin();

	Serial.println("\n=== Web Server Ready! ===");
	Serial.print("Visit: http://");
	Serial.println(WiFi.localIP());
	Serial.println("Waiting for client");

	initFastLeds();
	FastLED.setBrightness(lightBrightness);
	FastLED.show();

	currentMode = AnimationMode::ManualSet;
	chooseNextRandomAnimation();
}

void AnimationWebServer::startAnimationLoop()
{
	if(NUM_ANIMATIONS <= 0)
	{
		debugMessage("No animations, so exiting animation loop");
		return;
	}

	debugMessage("Starting animation loop");

	while(true)
	{
		switch(currentMode)
		{
			case AnimationMode::ManualSet:
				delayUnlessInterrupted(100000);
				debugMessage("Doing manual animation");
			break;
			case AnimationMode::PlainColor:
				delayUnlessInterrupted(100000);
				debugMessage("Doing plain color animation");
			break;
			case AnimationMode::HoldAnimation:
				debugMessage("Doing hold animation");
				startOfCurrentAnimation = millis();
				ANIMATIONS[currentAnimationIndex]();
			break;

			case AnimationMode::RandomAnimation:
				debugMessage("Doing RandomAnimation ");
				startOfCurrentAnimation = millis();
				ANIMATIONS[currentAnimationIndex]();
				// If we're still in the rotation mode, increment
				if(currentMode == AnimationMode::RandomAnimation)
					chooseNextRandomAnimation();
			break;
		}
	}
}

void AnimationWebServer::chooseNextRandomAnimation()
{
	currentAnimationIndex = getNextRandomExclusive(currentAnimationIndex, NUM_ANIMATIONS);
	toggleLedDirection();
	debugValue("Newly picked animation index", currentAnimationIndex);
}

bool AnimationWebServer::delayUnlessInterrupted(int delayMillieseconds)
{
	ulong startTime = millis();
	while(millis() - startTime < delayMillieseconds)
	{
		server.handleClient();
	}

	return false;
}

bool AnimationWebServer::shouldCurrentAnimationContinue()
{
	return currentMode == AnimationMode::HoldAnimation || millis() - startOfCurrentAnimation < animationMilliSeconds;
}

void AnimationWebServer::setStrandColor(Color color)
{
	/*
	plainStrandColor = color;
	for(int i = 0; i < NUM_STRAND_LEDS; i++)
	{
		setLed(i, plainStrandColor);
	}
	currentMode = AnimationMode::PlainColor;
	FastLED.show();
	*/
}

void AnimationWebServer::setStrandBrightness(int brightness)
{
	lightBrightness = clamp(brightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
	FastLED.setBrightness(lightBrightness);
	FastLED.show();
}

void AnimationWebServer::printToLed(const String text)
{
	debugMessage(text);
	/*
	matrix.beginDraw();
	
	matrix.stroke(0xFFFFFFFF);
	matrix.textScrollSpeed(50);
	
	matrix.textFont(Font_5x7);
	matrix.beginText(12, 1, 0xFFFFFF);
	matrix.println(text);
	matrix.endText(SCROLL_LEFT);
	
	matrix.endDraw();
	*/
}

// bool AnimationWebServer::getPixel(int column, int row) 
// {
// 	return leds[row * ledMatrixWidth + column];
// }

// bool AnimationWebServer::setPixel(int column, int row, bool value) 
// {
// 	return leds[row * ledMatrixWidth + column] = value;
// }

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

void AnimationWebServer::handleLedMessage(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	// Check for message parameter in query string
	for (int i = 0; i < params.count; i++) 
	{
		if (String(params.params[i].key) == "message") 
		{
			String message = params.params[i].value;
			Serial.print("message: ");
			Serial.println(message);

			printToLed(message);
			break;
		}
	}

	server.sendResponse(client, HTML_SET_MESSAGE_CONTENT, MIME_HTML_TYPE);
}

void AnimationWebServer::handlePointLeds(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	if (method == "GET") 
	{
		server.sendResponse(client, HTML_SET_LEDS_CONTENT, MIME_HTML_TYPE);
	}
}

void AnimationWebServer::handleSetManualColorRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	// TODO: Go into manual mode

	if (method == "POST") 
	{
		// Some sanity checking
		if(jsonData.length() < 50)
		{
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
	}
}

void AnimationWebServer::handleSetAllLedsColorRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	if (method == "POST") 
	{
		// Some sanity checking
		if(jsonData.length() < 50)
		{
			int endOfRIndex = jsonData.indexOf(',');
			int endOfGIndex = jsonData.indexOf(',', endOfRIndex + 1);
			String rValueString = jsonData.substring(0, endOfRIndex);
			String gValueString = jsonData.substring(endOfRIndex + 1, endOfGIndex);
			String bValueString = jsonData.substring(endOfGIndex + 1);
			int r = rValueString.toInt();
			int g = gValueString.toInt();
			int b = bValueString.toInt();

			debugValue("Json recieved", jsonData);
			debugValue("R", rValueString);
			debugValue("G", gValueString);
			debugValue("B", bValueString);
			
			for(int bulbIndex = 0; bulbIndex < NUM_STRAND_LEDS; bulbIndex++)
				bulbs[bulbIndex].color = Color(r, g, b);
			refreshStrandFromBulbInfo();
		}

		//server.sendResponse(client, "");
	}
}

void AnimationWebServer::handleSetRepeatedColorRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	if (method == "POST") 
	{
		// Some sanity checking
		if(jsonData.length() < 50)
		{
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
			// TODO: Some kind of refresh
		}
	}
}

void AnimationWebServer::handleSetNumRepeatedColorsRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	if (method == "POST") 
	{
		// Some sanity checking
		if(jsonData.length() < 50)
		{
			repeatingColorsToUse = jsonData.toInt();
			// TODO: Some kind of refresh
		}
	}
}

void AnimationWebServer::handleLedControllerStateRequested(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	const int maxMatrixStringSize =  sizeof("[1,") * NUM_STRAND_LEDS * 5 /*Fudge factor*/;
	char stringBuffer[50]; 

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

	// Add Brightness
	sprintf(stringBuffer, "\"brightness\" : %d}", lightBrightness); 
	jsonBuilder += stringBuffer;

	server.sendResponse(client, jsonBuilder.c_str(), MIME_JASON_TYPE);
}

void AnimationWebServer::handleNotFound(WiFiClient& client, const String& method, const String& request, const QueryParams& params, const String& jsonData) 
{
	SUPPRESS_WEB_HANDLER_UNUSED_VARIABLE_WARNING();

	server.sendResponse(client, HTML_CONTENT_404, MIME_HTML_TYPE);
}