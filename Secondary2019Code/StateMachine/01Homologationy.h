/*
 * Homologationy.h
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_HOMOLOGATIONY_H_
#define STATEMACHINE_HOMOLOGATIONY_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class Homologationy : public AbstractState {
public:
	Homologationy();
	virtual ~Homologationy();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:
	int trajectory_index = 0;
	unsigned long time_start;
	USDistances usDistances;
};

extern Homologationy homologationy;


#endif /* STATEMACHINE_HOMOLOGATIONY_H_ */
