/*
 * TurnToButtonState.h
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_TURNTOBUTTONSTATE_H_
#define STATEMACHINE_TURNTOBUTTONSTATE_H_

#include "AbstractState.h"

class TurnToButtonState : public AbstractState {
public:
	TurnToButtonState();
	virtual ~TurnToButtonState();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:

	unsigned long time_servo;
	unsigned long time_start;
};

extern TurnToButtonState turnToButtonState;

#endif /* STATEMACHINE_TURNTOBUTTONSTATE_H_ */
