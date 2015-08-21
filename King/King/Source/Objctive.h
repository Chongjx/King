#ifndef OBJECTIVE_H
#define OBJECTIVE_H

class Objective
{
public:
	enum OBJECTIVE_STATE:
	{
		STATE_UNASSIGNED,
		STATE_ASSIGNED,
		STATE_INPROGRESS,
		STATE_COMPLETED,
		MAX_STATE,
	};

	Objective(void);
	~Objective(void);

	void setDifficulty(int difficulty);
	void setState(OBJECTIVE_STATE state);

	int getDifficulty(void) const;
	OBJECTIVE_STATE getObjectiveState(void) const;
private:
	int difficulty;
	OBJECTIVE_STATE currentState;
};

#endif