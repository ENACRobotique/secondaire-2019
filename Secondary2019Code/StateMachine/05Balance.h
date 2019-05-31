/*
 * Balance.h
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_BALANCE_H_
#define STATEMACHINE_BALANCE_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class Balance : public AbstractState {
public:
	Balance();
	virtual ~Balance();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:
	int trajectory_index;
	unsigned long time_start;
	USDistances usDistances;
};

extern Balance balance;


#endif /* STATEMACHINE_BALANCE_H_ */
