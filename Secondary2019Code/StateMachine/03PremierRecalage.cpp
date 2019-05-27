/*
 * PremierRecalage.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "02PremierRangement.h"
#include "03PremierRecalage.h"
#include "00TiretteState.h"
#include "04RecolteChaos.h"
#include "DeadState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "../odometry.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"

PremierRecalage premierRecalage = PremierRecalage();


float traj_recalage1_purple[][3] = { {DISPLACEMENT,650,550},
									{TURN,135,0},
									{TURN,90,0},
									{DISPLACEMENT,650,780},
									{TURN,45,0},
									{TURN,0,0},
									{DISPLACEMENT,-15,780}
};


float traj_recalage1_yellow[][3] = { {DISPLACEMENT,2350,550},
									{TURN,45,0},
									{TURN,90,0},
									{DISPLACEMENT,2350,780},
									{TURN,135,0},
									{TURN,180,0},
									{DISPLACEMENT,3015,780}
};



PremierRecalage::PremierRecalage() {
	trajectory_index = 0;
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = 80;
	angles.angleB = 100;
}

PremierRecalage::~PremierRecalage() {
	// TODO Auto-generated destructor stub
}

void PremierRecalage::enter() {
	Serial.println("Onj entre dans enter de premierRecalage");
	//Odometry::set_pos(220, 550, -180);
	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recalage1_purple[trajectory_index][1],traj_recalage1_purple[trajectory_index][2]);
	}
	else{
		navigator.move_to(traj_recalage1_yellow[trajectory_index][1],traj_recalage1_yellow[trajectory_index][2]);
	}
	//Serial.println("Etat premiere recalage");


}

void PremierRecalage::leave() {

}

void PremierRecalage::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(trajectory_index == 6){
			if(tiretteState.get_color() == PURPLE){
				Odometry::set_pos(25,780,0);
			}
			else{
				Odometry::set_pos(2975,780,0);
			}
			fsmSupervisor.setNextState(&recolteChaos);
			//fsmSupervisor.setNextState(&deadState);
		}
		else{
			if(tiretteState.get_color() == PURPLE){
				trajectory_index += 1;
				if(traj_recalage1_purple[trajectory_index][0]==DISPLACEMENT)
					navigator.move_to(traj_recalage1_purple[trajectory_index][1],traj_recalage1_purple[trajectory_index][2]);
				else if(traj_recalage1_purple[trajectory_index][0]==TURN)
					navigator.turn_to(traj_recalage1_purple[trajectory_index][1] );
			}
			else{
				trajectory_index += 1;
				if(traj_recalage1_yellow[trajectory_index][0]==DISPLACEMENT)
					navigator.move_to(traj_recalage1_yellow[trajectory_index][1],traj_recalage1_yellow[trajectory_index][2]);
				else if(traj_recalage1_yellow[trajectory_index][0]==TURN)
					navigator.turn_to(traj_recalage1_yellow[trajectory_index][1] );
			}
		}
	}
}

void PremierRecalage::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	/*if(digitalRead(COLOR) == GREEN){
		navigator.move_to(POS_X_WATER,POS_Y_WATER_GREEN);
	}
	else{
		navigator.move_to(POS_X_WATER,POS_Y_WATER_ORANGE);
	}*/
	Serial.println("reenter");
	/*if(digitalRead(COLOR) == GREEN){
		navigator.move_to(1500,-10000);
	}
	else{
		navigator.move_to(1500,-10000);
	}*/
}

void PremierRecalage::forceLeave(){

}

