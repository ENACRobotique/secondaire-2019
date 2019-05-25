/*
 * MoveToButtonState.h
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_MOVETOBUTTONSTATE_H_
#define STATEMACHINE_MOVETOBUTTONSTATE_H_

#include "AbstractState.h"
#include "../lib/USManager.h"

class MoveToButtonState : public AbstractState {
public:
	MoveToButtonState();
	virtual ~MoveToButtonState();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:

	unsigned long time_start;
	int trajectory_index;
	unsigned long time_servo;
	USDistances usDistances;
};

extern MoveToButtonState moveToButtonState;

#endif /* STATEMACHINE_MOVETOBUTTONSTATE_H_ */
