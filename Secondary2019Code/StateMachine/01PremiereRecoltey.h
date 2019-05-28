/*
 * PremiereRecoltey.h
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_PREMIERERECOLTEY_H_
#define STATEMACHINE_PREMIERERECOLTEY_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class PremiereRecoltey : public AbstractState {
public:
	PremiereRecoltey();
	virtual ~PremiereRecoltey();

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

extern PremiereRecoltey premiereRecoltey;


#endif /* STATEMACHINE_PREMIERERECOLTEY_H_ */
