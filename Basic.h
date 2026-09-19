#pragma once
#include "Configuration.h"
#include "FastLED.h"


const byte BYTE_MAX = 255;
const byte BYTE_MIN = 0;

#ifdef DEBUG
	#define assertPrint(flag, message)\
		if(!(flag)) \
		{\
			delay(1000);\
			Serial.println((message));\
		}

	#define debug(message)\
		Serial.println((message));

	#define debugValue(header, value)\
		Serial.print(header);\
		Serial.print(": ");\
		Serial.println((value));

	#define debugRaw(value) debugValue(#value, value);

	#define debugValueIf(flag, header, value)\
		if(flag)\
		{\
			debugValue(header, value);\
		}\

	#define debugBlink(on)\
		Serial.print("Blink: ");\
		Serial.println(on ? "On" : "Off");\
		if(on)\
			digitalWrite(LED_BUILTIN, HIGH);\
		else\
			digitalWrite(LED_BUILTIN, LOW);\
		delay(1000);\
		digitalWrite(LED_BUILTIN, LOW);\
		delay(1000);\


#else
	#define assertPrint(flag, message)
	#define debug(message)
	#define debugValue(header, value)
	#define debugValueIf(flag, header, value)
	#define debugRaw(value) debugValue(#value, value);
	#define debugBlink(on)

#endif

#define getStaticArraySize(array) sizeof(array) / sizeof(array[0])

void initDebugIO();


template<typename numeric>
numeric clamp(numeric value,numeric minValue, numeric maxValue)
{
	if(value < minValue)
		return minValue;
	else if(value > maxValue)
		return maxValue;
	else
		return value;
}

template<typename numeric>
byte clampToByte(numeric v)
{
	if(v < 0)
		return BYTE_MIN;
	else if(v > BYTE_MAX)
		return BYTE_MAX;
	else
		return (byte)v;
}

template<typename numeric>
numeric lerp(float t, numeric start, numeric stop)
{
	return start * (1 - t) + stop * t;
}

template<typename numeric>
numeric min3(numeric a, numeric b, numeric c)
{
    return min(min(a, b), c);
}

int getNextRandomExclusive(int start, int exclusiveMax);

int random(int inclusiveMin, int exclusiveMax);

bool randomBool();

typedef unsigned long Timestamp;
typedef CRGB Color;
typedef unsigned long ulong;

// template<typename TypeOne, typename TypeTwo>
// struct Pair
// {
// 	public:
// 	TypeOne One;
// 	TypeTwo Two;

// 	Pair(TypeOne one, TypeTwo two)
// 	{
// 		One = one;
// 		Two = two;
// 	}
// };