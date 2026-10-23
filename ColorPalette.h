#pragma once
#include "Basic.h"

class ColorPalette
{
	const Color* colors = nullptr;
	int numColors = 0;

public:
	ColorPalette(const Color* colors, int numColors);
	int getNumColors();
	Color getColor(int colorIndex) const;
	void updateColors(const Color* colors, int numColors);
};
