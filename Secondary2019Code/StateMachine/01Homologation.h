/*
 * PremiereRecolte.h
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */

#ifndef STATEMACHINE_HOMOLOGATION_H_
#define STATEMACHINE_HOMOLOGATION_H_


#include "AbstractState.h"
#include "../lib/USManager.h"


class Homologation : public AbstractState {
public:
	Homologation();
	virtual ~Homologation();

	void doIt();
	void leave();
	void enter();
	void reEnter(unsigned long interruptTime);
	void forceLeave();
	unsigned long get_time_start(){
		return time_start;
	}

private:
	int trajectory_index = 0;
	unsigned long time_start;
	USDistances usDistances;
};

extern Homologation homologation;


#endif /* STATEMACHINE_HOMOLOGATION_H_ */
