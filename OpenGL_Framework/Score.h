#pragma once

struct ScoreCounter
{
	static int coinCount;
	static int acornCount;
	float totalGameTime = 0.0f;

	int getCoinCount() { return coinCount; }
	void plusCoin() { coinCount++; }
	void voidCoin() { coinCount = 0; }

	int getAcornCount() { return acornCount; }
	void plusAcorn() { acornCount++; }
	void voidAcorn() { acornCount = 0; }

	float getTotalGameTime() { return totalGameTime; }
	void setTotalGameTime(float deltaTime) { totalGameTime += deltaTime; }
	void voidTotalGameTime() { totalGameTime = 0.0f; }
};