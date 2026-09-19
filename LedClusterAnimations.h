// #pragma once
// #include "ColorUtils.h"
// #include "Basic.h"
// #include "ColorPalette.h"
// #include "ColorPairPalette.h"
// // Contains sets of animations for animating a subset of a string of led lights.

// enum AnimationKey
// {
// 	Rainbow,
// 	EndAnimation,
// };

// enum AnimationState
// {
// 	InitializingWarmup,
// 	WarmingUp,
// 	InitializingAnimation,
// 	Animating,
// };

// class LedClusterAnimator
// {
// public:

// 	int startLedIndex;
// 	int endLedIndex;
// 	int ledCount;
// 	AnimationKey currentAnimationKey;
// 	Timestamp animationStartTime;
// 	Timestamp animationEndTime;
// 	AnimationState animationState;
// 	ColorPalette colorGenerator = COMPLEX_RAINBOW_COLOR_PALETTE;
// 	Color* startWarmupColors;
// 	Color* endWarmupColors;

// 	// Rainbow animation state
// 	Color startColor;
// 	Color endColor;
// 	int numTransitionsLeft = 0;
// 	int colorChangeSpeed;
// 	const static int MAX_RAINBOW_TRANSITIONS = 800000;

// 	LedClusterAnimator(int startIndex, int endIndex)
// 	{
// 		this->startLedIndex = startIndex;
// 		this->endLedIndex = endIndex;
// 		this->ledCount = endLedIndex - startLedIndex + 1;

// 		this->currentAnimationKey = AnimationKey::Rainbow;
// 		this->animationState = AnimationState::InitializingWarmup;
// 	}

// 	void setNextAnimation(AnimationKey nextKey)
// 	{
// 		// TODO: USe this
// 		// this->currentAnimationKey = nextKey;
// 		// this->animationNeedsWarmup = true;
// 	}

// 	float getTimeBasedLerpValue(Timestamp currentTime)
// 	{
// 		float lerpValue = (currentTime - animationStartTime) / (float)(animationEndTime - animationStartTime);
// 		return clamp(lerpValue, 0.0f, 1.0f);
// 	}

// 	void startWarmup()
// 	{
// 		assertPrint(startWarmupColors == nullptr, "These should've been cleaned up");

// 		startWarmupColors = new Color[ledCount];
// 		for(int ledIndex = 0; ledIndex < ledCount; ledIndex++)
// 		{
// 			startWarmupColors[ledIndex] = getLed(ledIndex + startLedIndex);
// 		}

// 		this->animationStartTime = millis();
// 		this->animationEndTime = animationStartTime + 5000;
// 	}

// 	void tickWarmup()
// 	{
// 		Timestamp currentTime = millis();
// 		float lerpValue = getTimeBasedLerpValue(currentTime);

// 		for(int ledIndex = 0; ledIndex < ledCount; ledIndex++)
// 		{
// 			Color currentColor = colorLerp(lerpValue, startWarmupColors[ledIndex], endWarmupColors[ledIndex]);
// 			setLed(ledIndex + startLedIndex, currentColor);
// 		}

// 		if(currentTime > animationEndTime)
// 		{
// 			incrementAnimationState();
// 			delete[] startWarmupColors;
// 			startWarmupColors = nullptr;
// 			delete[] endWarmupColors;
// 			endWarmupColors = nullptr;
// 		}
// 	}

// 	void nextRainbowColor()
// 	{
// 		this->startColor = this->endColor;
// 		this->endColor = colorGenerator.getNextRandomColor();
// 		numTransitionsLeft--;
// 		this->animationStartTime = millis();
// 		this->animationEndTime = animationStartTime + colorChangeSpeed;
// 	}

// 	void initializeRainbowAnimation()
// 	{
// 		nextRainbowColor();
// 		this->numTransitionsLeft = MAX_RAINBOW_TRANSITIONS;
// 		colorChangeSpeed = random(1000, 5000);

// 		endWarmupColors = new Color[ledCount];
// 		for(int ledIndex = 0; ledIndex < ledCount; ledIndex++)
// 			endWarmupColors[ledIndex] = endColor;
// 		startWarmup();
// 	}

// 	void tickRainbowAnimation()
// 	{
// 		if(this->animationState == AnimationState::InitializingAnimation)
// 		{
// 			incrementAnimationState();
// 			nextRainbowColor();
// 		}

// 		Timestamp currentTime = millis();
// 		float lerpValue = getTimeBasedLerpValue(currentTime);
// 		Color currentColor = colorLerp(lerpValue, startColor, endColor);

// 		for(int ledIndex = 0; ledIndex < ledCount; ledIndex++)
// 			setLed(ledIndex + startLedIndex, currentColor);

// 		if(currentTime > animationEndTime)
// 		{
// 			nextRainbowColor();
// 		}

// 		if(numTransitionsLeft <= 0)
// 		{
// 			incrementAnimationState();
// 		}
// 	}

// 	void incrementAnimationState()
// 	{
// 		this->animationState = (int)this->animationState + 1;
// 		if(animationState > AnimationState::Animating)
// 			animationState = AnimationState::InitializingWarmup;
// 	}

// 	void initalizeAndStartWarmup()
// 	{
// 		switch(this->currentAnimationKey)
// 		{
// 			case AnimationKey::Rainbow:
// 				assertPrint(isBlack(getLed(48)), "Should be");
// 				initializeRainbowAnimation();
// 				break;
// 			default:
// 				assertPrint(false, "Unhandled animation")
// 				break;
// 		}

// 		incrementAnimationState();
// 	}

// 	void animate()
// 	{
// 		switch(this->currentAnimationKey)
// 		{
// 			case AnimationKey::Rainbow:
// 				tickRainbowAnimation();
// 				break;
// 			default:
// 				assertPrint(false, "Unhandled animation")
// 				break;
// 		}
// 	}

// 	void tick()
// 	{
// 		switch(this->animationState)
// 		{
// 			case InitializingWarmup:
// 				initalizeAndStartWarmup();
// 				break;
// 			case WarmingUp:
// 				tickWarmup();
// 				break;
// 			case InitializingAnimation:
// 			case Animating:
// 				animate();
// 				break;
// 		}
// 	}
// };

// void doLedClusterAnimations(LedClusterAnimator clusterAnimators[], int numAnimators)
// {
// 	for(int animatorIndex = 0; animatorIndex < numAnimators; animatorIndex++)
// 	{
// 		LedClusterAnimator& animator = clusterAnimators[animatorIndex];
// 		animator.tick();
// 	}

// 	FastLED.show();
// }