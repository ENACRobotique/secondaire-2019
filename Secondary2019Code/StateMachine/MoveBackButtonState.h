/*
 * MoveBackButtonState.h
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_MOVEBACKBUTTONSTATE_H_
#define STATEMACHINE_MOVEBACKBUTTONSTATE_H_

#include "AbstractState.h"
#include "../lib/USManager.h"

class MoveBackButtonState : public AbstractState {
public:
	MoveBackButtonState();
	virtual ~MoveBackButtonState();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:
	USDistances usDistances;
	unsigned long time_start;

};

extern MoveBackButtonState moveBackButtonState;

#endif /* STATEMACHINE_MOVEBACKBUTTONSTATE_H_ */
