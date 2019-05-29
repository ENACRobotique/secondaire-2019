/*
 * HomoRangement.h
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_HOMORANGEMENT_H_
#define STATEMACHINE_HOMORANGEMENT_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class HomoRangement : public AbstractState {
public:
	HomoRangement();
	virtual ~HomoRangement();

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

extern HomoRangement homoRangement;


#endif /* STATEMACHINE_HOMORANGEMENT_H_ */
