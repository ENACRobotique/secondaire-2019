/*
 * RetractArmButtonState.h
 *
 *  Created on: 18 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_RETRACTARMBUTTONSTATE_H_
#define STATEMACHINE_RETRACTARMBUTTONSTATE_H_

#include "AbstractState.h"
#include "../lib/USManager.h"

class RetractArmButtonState : public AbstractState {
public:
	RetractArmButtonState();
	virtual ~RetractArmButtonState();

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

extern RetractArmButtonState retractArmButtonState;

#endif /* STATEMACHINE_RETRACTARMBUTTONSTATE_H_ */
