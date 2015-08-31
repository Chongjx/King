#include "Score.h"

Score::Score(void):
score(0),
place(0)
{

}

Score::~Score(void)
{
}
void Score::setScore (int score)
{
	this->score=score;
}
void Score::setplace (int place)
{
	this->place=place;
}

int Score::getScore (void)
{
	return score;
}
int Score::getplace (void)
{
	return place;
}
