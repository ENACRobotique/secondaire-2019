/*
 * MoveToConstructState.h
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_MOVETOCONSTRUCTSTATE_H_
#define STATEMACHINE_MOVETOCONSTRUCTSTATE_H_

#include "AbstractState.h"
#include "../lib/USManager.h"

class MoveToConstructState : public AbstractState {
public:
	MoveToConstructState();
	virtual ~MoveToConstructState();

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
	USDistances usDistances;
};

extern MoveToConstructState moveToConstructState;

#endif /* STATEMACHINE_MOVETOCONSTRUCTSTATE_H_ */
