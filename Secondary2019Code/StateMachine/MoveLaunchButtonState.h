/*
 * MoveLaunchButtonState.h
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_MOVELAUNCHBUTTONSTATE_H_
#define STATEMACHINE_MOVELAUNCHBUTTONSTATE_H_

#include "AbstractState.h"
#include "../lib/USManager.h"

class MoveLaunchButtonState : public AbstractState {
public:
	MoveLaunchButtonState();
	virtual ~MoveLaunchButtonState();

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
	unsigned long time_servo;
	USDistances usDistances;
};

extern MoveLaunchButtonState moveLaunchButtonState;

#endif /* STATEMACHINE_MOVELAUNCHBUTTONSTATE_H_ */
