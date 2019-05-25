/*
 * MoveBackConstructState.h
 *
 *  Created on: 20 avr. 2018
 *      Author: Maxime
 */

#ifndef STATEMACHINE_MOVEBACKCONSTRUCTSTATE_H_
#define STATEMACHINE_MOVEBACKCONSTRUCTSTATE_H_

#include "AbstractState.h"
#include "../lib/USManager.h"

class MoveBackConstructState : public AbstractState {
public:
	MoveBackConstructState();
	virtual ~MoveBackConstructState();

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

extern MoveBackConstructState moveBackConstructState;

#endif /* STATEMACHINE_MOVEBACKCONSTRUCTSTATE_H_ */
