/*
 * PauseState.h
 *
 *  Created on: 19 avr. 2018
 *      Author: robot
 */

#ifndef STATEMACHINE_PAUSESTATE_H_
#define STATEMACHINE_PAUSESTATE_H_
#include "AbstractState.h"

class PauseState : public AbstractState {
public:
	PauseState();
	virtual ~PauseState();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long getPauseTime();

private:
	unsigned long pauseStartTime;
};

extern PauseState pauseState;
#endif /* STATEMACHINE_PAUSESTATE_H_ */
