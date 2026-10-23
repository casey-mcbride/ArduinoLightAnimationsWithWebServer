#pragma once
#include "Basic.h"
#include "ColorPalette.h"
#include "ColorPairPalette.h"

class ColorIterator
{
private:
	int colorIndex = 0;
	int colorPairIndex = 0;

	ColorPalette* singlePalette = nullptr;
	ColorPairPalette* pairPalette = nullptr;

public:

	ColorPalette* getPalette() const;
	ColorPairPalette* getPairPalette() const;
	void setPalette(ColorPalette* palette);
	void setPairPalette(ColorPairPalette* palette);

	Color getNextColor();
	Color getNextRandomColor();
	Color getRandomColor();

	void getNextColorPair(Color& color1, Color& color2);
	void getNextRandomColorPair(Color& color1, Color& color2);
};
