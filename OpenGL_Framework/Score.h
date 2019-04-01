#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using std::string;
using std::vector;
using std::ifstream;// is for reading
using std::ofstream;//is for writing
using std::getline;
using std::to_string;
using std::sort;

struct ScoreCounter
{
	static int coinCount;
	static int acornCount;
	static float totalGameTime;
	string playerName;
	

	int getCoinCount() { return coinCount; }
	void plusCoin() { coinCount++; }
	void voidCoin() { coinCount = 0; }

	int getAcornCount() { return acornCount; }
	void plusAcorn() { acornCount++; }
	void voidAcorn() { acornCount = 0; }

	float getTotalGameTime() { return totalGameTime; }
	void setTotalGameTime(float deltaTime) { totalGameTime += deltaTime; }
	void voidTotalGameTime() { totalGameTime = 0.0f; }

	void writeScores(string file, vector<ScoreCounter*> scores);
	vector<ScoreCounter*> readScores(string file);
	void sortScores(vector<ScoreCounter*> &scores);
};