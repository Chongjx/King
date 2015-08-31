#ifndef Score_H
#define Score_H

class Score
{
public:

	Score(void);
	~Score(void);

	void setScore (int);
	void setplace (int);

	int getScore (void);
	int getplace (void);

private:
	int score;
	int place;
};

#endif