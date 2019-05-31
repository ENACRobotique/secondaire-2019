/*
 * BasculePalets.h
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_BASCULEPALETS_H_
#define STATEMACHINE_BASCULEPALETS_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class BasculePalets : public AbstractState {
public:
	BasculePalets();
	virtual ~BasculePalets();

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

extern BasculePalets basculePalets;


#endif /* STATEMACHINE_BASCULEPALETS_H_ */
