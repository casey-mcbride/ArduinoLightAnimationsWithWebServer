#include "ColorUtils.h"

static Color leds[NUM_STRAND_LEDS];
static bool isLedDirectionSwapped = false;

void toggleLedDirection()
{
	isLedDirectionSwapped = !isLedDirectionSwapped;
}

void initFastLeds()
{
	FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, NUM_STRAND_LEDS);
}

void setLed( int index, Color color)
{
	if(isLedDirectionSwapped)
		index = NUM_STRAND_LEDS - (index + 1);

	assertPrint(index >= 0 && index < NUM_STRAND_LEDS, "LED index out of bounds");
	index = clamp(index, 0, NUM_STRAND_LEDS - 1);
	leds[index] = color;
}

Color getLed(int index)
{
	if(isLedDirectionSwapped)
		index = NUM_STRAND_LEDS - (index + 1);

	assertPrint(index >= 0 && index < NUM_STRAND_LEDS, "LED index out of bounds");
	index = clamp(index, 0, NUM_STRAND_LEDS - 1);
	return leds[index];
}

Color dimColor(Color color, byte dimAmount)
{
	byte r = color.r > dimAmount ? color.r - dimAmount : 0;
	byte g = color.g > dimAmount ? color.g - dimAmount : 0;
	byte b = color.b > dimAmount ? color.b - dimAmount : 0;

	return Color(r, g, b);
}

bool isBlack(Color color)
{
	return color.r == 0 && color.g == 0 && color.b == 0 ;
}

Color colorLerp(float lerpValue, Color color1, Color color2)
{
	lerpValue = clamp(lerpValue, 0.0f, 1.0f);

	byte r = lerp(lerpValue, color1.r, color2.r);
	byte g = lerp(lerpValue, color1.g, color2.g);
	byte b = lerp(lerpValue, color1.b, color2.b);

	return Color(clamp(r, BYTE_MIN, BYTE_MAX), clamp(g, BYTE_MIN, BYTE_MAX), clamp(b, BYTE_MIN, BYTE_MAX));
}

Color colorAverage(Color color1, Color color2, Color color3, float weight1, float weight2, float weight3)
{
	float weightSum = weight1 + weight2 + weight3;
	float r = (color1.r * weight1 + color2.r * weight2 + color3.r * weight3) / weightSum;
	float g = (color1.g * weight1 + color2.g * weight2 + color3.g * weight3) / weightSum;
	float b = (color1.b * weight1 + color2.b * weight2 + color3.b * weight3) / weightSum;

	Serial.println(r);

	return Color(clampToByte((int)r), clampToByte((int)g), clampToByte((int)b));
}