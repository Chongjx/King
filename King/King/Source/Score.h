#ifndef Score_H
#define Score_H

class Score
{
public:

	Score(void);
	~Score(void);

	void setScore(int,int,int);
	void setScoreD (int);
	void setScoreH (int);
	void setScoreM (int);


	int getScoreD (void);
	int getScoreH (void);
	int getScoreM (void);


private:
	int scoreD;
	int scoreH;
	int scoreM;
	int place;
};

#endif