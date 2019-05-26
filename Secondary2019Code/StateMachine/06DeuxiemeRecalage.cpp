/*
 * deuxiemeRecalage.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */



#include "05RangementChaos.h"
#include "06DeuxiemeRecalage.h"
#include "00TiretteState.h"
//#include "07AtomeMontee.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "../odometry.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"

DeuxiemeRecalage deuxiemeRecalage = DeuxiemeRecalage();


float traj_recalage2_purple[][3] = { {DISPLACEMENT,600,400},
									{TURN,135,0},
									{TURN,90,0},
									{DISPLACEMENT,600,1400},

};

float traj_recalage2_yellow[][3] = { {DISPLACEMENT,2400,400},
									{TURN,135,0},
									{TURN,90,0},
									{DISPLACEMENT,2400,1400},

};



DeuxiemeRecalage::DeuxiemeRecalage() {
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

DeuxiemeRecalage::~DeuxiemeRecalage() {
	// TODO Auto-generated destructor stub
}

void DeuxiemeRecalage::enter() {

	//Serial.println("Etat premiere recalage");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recalage2_purple[0][1],traj_recalage2_purple[0][2]);
	}
	else{
		navigator.move_to(traj_recalage2_yellow[0][1],traj_recalage2_yellow[0][2]);
	}
}

void DeuxiemeRecalage::leave() {

}

void DeuxiemeRecalage::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(trajectory_index == 3){
			if(tiretteState.get_color() == PURPLE){
				Odometry::set_pos(600,1378,90);
			}
			else{
				Odometry::set_pos(25,780,0); //TODO regarder les mesure du cote jaune
			}
			//fsmSupervisor.setNextState(&atomeMontee);
		}
		else{
			if(tiretteState.get_color() == PURPLE){
				trajectory_index += 1;
				if(traj_recalage2_purple[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(traj_recalage2_purple[trajectory_index][1],traj_recalage2_purple[trajectory_index][2]);
				}
				else if(traj_recalage2_purple[trajectory_index][0]==TURN){
					navigator.turn_to(traj_recalage2_purple[trajectory_index][1] );
				}

			}
			else{
				//navigator.turn_to(turn_recalage1_yellow[trajectory_index]);
				trajectory_index += 1;
				//navigator.move_to(traj_recalage2_yellow[trajectory_index][1],traj_recalage2_yellow[trajectory_index][2]);
			}
		}
	}
}

void DeuxiemeRecalage::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	/*if(digitalRead(COLOR) == GREEN){
		navigator.move_to(POS_X_WATER,POS_Y_WATER_GREEN);
	}
	else{
		navigator.move_to(POS_X_WATER,POS_Y_WATER_ORANGE);
	}*/
	Serial.println("reenter");

}

void DeuxiemeRecalage::forceLeave(){

}

