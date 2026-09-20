#pragma once

#include "Basic.h"
#include "ColorPalette.h"
#include "ColorPairPalette.h"

const Color DIM_WHITE = Color(80, 80, 80);
const Color WHITE = Color(120, 120, 120);
const Color ORCHID = Color(255, 0, 100);
const Color HOT_PINK = Color::HotPink;

const Color ORANGE = Color::OrangeRed;
const Color GOLDEN_YELLOW = Color::Orange;
const Color PALE_YELLOW = Color::Goldenrod;

const Color YELLOW = Color::Yellow;
const Color PALE_GREEN_YELLOW = Color::YellowGreen;

const Color PEA_GREEN = Color(100, 200, 0);
const Color LIME_GREEN = Color::Green;

const Color SKY_BLUE = Color::SeaGreen;
const Color PALE_BLUE = Color::RoyalBlue;
const Color ROYAL_BLUE = Color::Blue;
const Color PURPLE_BLUE = Color(50, 0, 255);

const Color LAVENDAR = Color(100, 50, 255);
const Color PURPLE = Color::Purple;
const Color PALE_PURPLE = Color(50, 0, 200);

const Color COMPLEX_RAINBOW[] =
{
	// Reds
	ORCHID,
	// Color::Red,
	HOT_PINK,

	// Oranges
	ORANGE,
	GOLDEN_YELLOW,
	PALE_YELLOW,

	// Yellows
	YELLOW,
	PALE_GREEN_YELLOW,

	// Greens
	PEA_GREEN,
	LIME_GREEN,

	SKY_BLUE,
	PALE_BLUE,
	ROYAL_BLUE,
	PURPLE_BLUE,

	LAVENDAR,
	PURPLE,
	PALE_PURPLE,
};
const int COMPLEX_RAINBOW_COLOR_COUNT = getStaticArraySize(COMPLEX_RAINBOW);

const Color RAINBOW_PAIRS[] =
{
	// ORCHID,
	// ORANGE,

	ORCHID,
	ROYAL_BLUE,

	ORCHID,
	LAVENDAR,

	ORCHID,
	PURPLE_BLUE,

	ORCHID,
	PURPLE,

	HOT_PINK,
	ROYAL_BLUE,

	// HOT_PINK,
	// PURPLE_BLUE,

	HOT_PINK,
	PURPLE,

	// HOT_PINK,
	// PALE_PURPLE,

	// ORANGE,
	// YELLOW,

	ORANGE,
	PURPLE,

	// YELLOW,
	// SKY_BLUE,

	YELLOW,
	LIME_GREEN,

	// PALE_YELLOW,
	// SKY_BLUE,

	PALE_GREEN_YELLOW,
	SKY_BLUE,

	PEA_GREEN,
	SKY_BLUE,

	PEA_GREEN,
	ROYAL_BLUE,

	LIME_GREEN,
	SKY_BLUE,

	LIME_GREEN,
	ROYAL_BLUE,

	LIME_GREEN,
	PALE_BLUE,

	SKY_BLUE,
	LAVENDAR,

	SKY_BLUE,
	PALE_PURPLE,

	ROYAL_BLUE,
	PURPLE,

	// ROYAL_BLUE,
	// PURPLE_BLUE,
};
const int NUM_RAINBOW_PAIRS = getStaticArraySize(RAINBOW_PAIRS) / 2;
static ColorPalette COMPLEX_RAINBOW_COLOR_PALETTE(COMPLEX_RAINBOW, COMPLEX_RAINBOW_COLOR_COUNT);
static ColorPairPalette COMPLEX_RAINBOW_COLOR_PAIR_PALETTE(RAINBOW_PAIRS, NUM_RAINBOW_PAIRS);

const Color HALLOWEEN_COLORS[] = {Color::Purple, Color::Orange, Color::Green, Color::Red};
const int NUM_HALLOWEEN_COLORS = getStaticArraySize(HALLOWEEN_COLORS);

const Color HALLOWEEN_PAIRS[] = 
{
	Color::Purple,
	Color::Red,

	Color::Purple,
	Color::Green,

	Color::Red,
	Color::Orange,

	Color::Green,
	Color::Orange,
};
const int NUM_HALLOWEEN_COLOR_PAIRS = getStaticArraySize(HALLOWEEN_PAIRS) / 2;
static ColorPalette HALLOWEEN_COLOR_PALETTE(HALLOWEEN_COLORS, NUM_HALLOWEEN_COLORS);
static ColorPairPalette HALLOWEEN_COLOR_PAIR_PALETTE(HALLOWEEN_PAIRS, NUM_HALLOWEEN_COLOR_PAIRS);

const Color THANKSGIVING_COLORS[] = {Color::Red, Color::Orange, Color::Yellow};
const int NUM_THANKSGIVING_COLORS = getStaticArraySize(THANKSGIVING_COLORS);

const Color THANKSGIVING_PAIRS[] = 
{
	Color::Yellow,
	Color::Red,

	Color::Red,
	Color::Orange,

	Color::Yellow,
	Color::Orange,
};
const int NUM_THANKSGIVING_COLOR_PAIRS = getStaticArraySize(THANKSGIVING_PAIRS) / 2;

static ColorPalette THANKSGIVING_COLOR_PALETTE(THANKSGIVING_COLORS, NUM_THANKSGIVING_COLORS);
static ColorPairPalette THANKSGIVING_COLOR_PAIR_PALETTE(THANKSGIVING_PAIRS, NUM_THANKSGIVING_COLOR_PAIRS);


const Color CHRISTMAS_COLORS[] = {Color::Red, Color::Green, Color::White, Color::Blue, Color::Purple, Color::Yellow};
const int NUM_CHRISTMAS_COLORS = getStaticArraySize(CHRISTMAS_COLORS);

const Color CHRISTMAS_PAIRS[] =
{
	Color::Green,
	Color::Red,

	Color::White,
	Color::Blue,

	Color::Red,
	Color::White,
};
const int NUM_CHRISTMAS_COLOR_PAIRS = getStaticArraySize(CHRISTMAS_PAIRS) / 2;

static ColorPalette CHRISTMAS_COLOR_PALETTE(CHRISTMAS_COLORS, NUM_CHRISTMAS_COLORS);
static ColorPairPalette CHRISTMAS_COLOR_PAIR_PALETTE(CHRISTMAS_PAIRS, NUM_CHRISTMAS_COLOR_PAIRS);
