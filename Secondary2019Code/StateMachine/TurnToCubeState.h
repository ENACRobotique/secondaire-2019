/*
 * TurnToCubeState.h
 *
 *  Created on: 18 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_TURNTOCUBESTATE_H_
#define STATEMACHINE_TURNTOCUBESTATE_H_

#include "AbstractState.h"

class TurnToCubeState : public AbstractState {
public:
	TurnToCubeState();
	virtual ~TurnToCubeState();

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
};

extern TurnToCubeState turnToCubeState;

#endif /* STATEMACHINE_TURNTOCUBESTATE_H_ */
