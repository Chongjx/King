#include "Score.h"

Score::Score(void):
scoreD(0),
	scoreH(0),
	scoreM(0)
{

}

Score::~Score(void)
{
}

void Score::setScore(int scoreD,int scoreH,int scoreM)
{
	this->scoreD=scoreD;
	this->scoreH=scoreH;
	this->scoreM = scoreM;
}
void Score::setScoreD (int scoreD)
{
	this->scoreD=scoreD;
}

int Score::getScoreD (void)
{
	return scoreD;
}

void Score::setScoreH (int scoreH)
{
	this->scoreH=scoreH;
}

int Score::getScoreH (void)
{
	return scoreH;
}

void Score::setScoreM (int scoreM)
{
	this->scoreM = scoreM;
}

int Score::getScoreM (void)
{
	return scoreM;
}
