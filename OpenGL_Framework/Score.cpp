#include "Score.h"

int ScoreCounter::acornCount = 0;
int ScoreCounter::coinCount = 0;
float ScoreCounter::totalGameTime = 0.0f;

void ScoreCounter::writeScores(string file, vector<ScoreCounter*> scores)
{
	ofstream writer;
	writer.open(file);

	unsigned int count;
	if (scores.size() >= 5)
		count = 5;
	else
		count = scores.size();

	if (writer.is_open())
	{
		for (unsigned int i = 0; i < count; i++)
		{
			writer << scores[i]->playerName + " " + to_string(scores[i]->totalGameTime) << std::endl;
		}
		writer.close();
	}
}

vector<ScoreCounter*> ScoreCounter::readScores(string file)
{
	ifstream reader;
	reader.open(file);

	vector<ScoreCounter*> temp;
	string line;
	string name = "";
	string score = "";

	if (reader.is_open())
	{
		while (getline(reader, line))
		{
			for (unsigned int i = 0; i < line.size(); i++)//this gets the name
			{
				if (line[i] == ' ')
					break;
				else
					name += line[i];
			}

			for (unsigned int i = name.size() + 1; i < line.size(); i++)//this gets the score
			{
				if (line[i] == ' ')
					break;
				else
					score += line[i];
			}
			ScoreCounter* temp_ = new ScoreCounter;
			temp_->playerName = name;
			temp_->totalGameTime = stof(score);
			temp.push_back(temp_);
		}
		reader.close();
	}
	return temp;
}

void ScoreCounter::sortScores(vector<ScoreCounter*> &scores)
{
	sort(scores.begin(), scores.end(), [](ScoreCounter* a, ScoreCounter* b) -> bool
	{
		return a->totalGameTime < b->totalGameTime;
	});
	//vector of scores
	//scores = read
	//sort(scores)
	//write (scores)
}
