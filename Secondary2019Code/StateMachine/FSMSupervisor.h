/*
 * FSMSupervisor.h
 *
 *  Created on: 11 avr. 2018
 *      Author: robot
 */

#ifndef STATEMACHINE_FSMSUPERVISOR_H_
#define STATEMACHINE_FSMSUPERVISOR_H_

#include "AbstractState.h"
#include "Metro.h"
#include "../lidar/LidarManager.h"

struct Angles zone_observation(int activation, float type_mouvement);


class FSMSupervisor {
public:
	FSMSupervisor();
	virtual ~FSMSupervisor();
	void setNextState(AbstractState* state);
	void update();
	void init(AbstractState * state);
	LidarManager lidarManager;

private:

	AbstractState* nextState;
	AbstractState* currentState;
	AbstractState* previousState;
	unsigned long time_obstacle_left;
	int deb;
	bool isLidarSet;
	unsigned int lidarPWM;
	Metro lidarAsservTime;

};

extern FSMSupervisor fsmSupervisor;

#endif /* STATEMACHINE_FSMSUPERVISOR_H_ */
