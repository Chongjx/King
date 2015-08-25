#ifndef OBJECTIVE_H
#define OBJECTIVE_H

#include <string>
using std::string;
class Objective
{
public:
	enum OBJECTIVE_STATE
	{
		OBJECTIVE_INPCOMPLETE,
		OBJECTIVE_COMPLETED,
		MAX_STATE,
	};

	Objective(void);
	~Objective(void);

	void initObjctives (string Title,bool Get,int level,string keyItem);
	void setState(OBJECTIVE_STATE state);
	void setTitle (string Title);
	void setGet (bool Get);
	void setlevel(int level);
	void setkeyItem (string keyItem);

	OBJECTIVE_STATE getObjectiveState(void) const;
	string getTitle (void)const;
	bool getGet (void) const;
	int getlevel (void)const;
	string getkeyItem (void) const;


private:
	OBJECTIVE_STATE currentState;
	string Title;
	bool Get;
	int level;
	string keyItem;
};

#endif