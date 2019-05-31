/*
 * MonteeAtome.h
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_MONTEEATOME_H_
#define STATEMACHINE_MONTEEATOME_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class MonteeAtome : public AbstractState {
public:
	MonteeAtome();
	virtual ~MonteeAtome();

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

extern MonteeAtome monteeAtome;


#endif /* STATEMACHINE_MONTEEATOME_H_ */
