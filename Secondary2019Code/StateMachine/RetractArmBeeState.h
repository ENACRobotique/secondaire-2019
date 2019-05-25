/*
 * RetractArmBeeState.h
 *
 *  Created on: 18 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_RETRACTARMBEESTATE_H_
#define STATEMACHINE_RETRACTARMBEESTATE_H_

#include "AbstractState.h"

class RetractArmBeeState : public AbstractState {
public:
	RetractArmBeeState();
	virtual ~RetractArmBeeState();

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

extern RetractArmBeeState retractArmBeeState;

#endif /* STATEMACHINE_RETRACTARMBEESTATE_H_ */
