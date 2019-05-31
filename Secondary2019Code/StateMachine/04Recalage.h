/*
 * Recalage.h
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_RECALAGE_H_
#define STATEMACHINE_RECALAGE_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class Recalage : public AbstractState {
public:
	Recalage();
	virtual ~Recalage();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:
	int trajectory_index;
	unsigned long time_start;
	USDistances usDistances;
};

extern Recalage recalage;


#endif /* STATEMACHINE_RECALAGE_H_ */
