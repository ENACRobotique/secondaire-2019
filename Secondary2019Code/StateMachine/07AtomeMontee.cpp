/*
 * AtomeMontee.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "06DeuxiemeRecalage.h"
#include "07AtomeMontee.h"
#include "00TiretteState.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "DeadState.h"
#include "../odometry.h"

AtomeMontee atomeMontee = AtomeMontee();

float traj_montee_purple[][4] = { {DISPLACEMENT, 600, 400, 1}, //{DISPLACEMENT, 600, 1200, 1},
								//{TURN,135,0, 1},
								//{TURN,180,0, 1},
								{DISPLACEMENT, 150, 1800, 1},
								{TURN, 70, 0, 0},
								{TURN, 0, 0, 0},
								//{DISPLACEMENT, 1100, 1800, 0},
								{DISPLACEMENT, 700, 1800, 0},
								{DISPLACEMENT, 1063, 1800, 0}};



float traj_montee_yellow[][4] = { {DISPLACEMENT, 2400, 400, 1},
								//{TURN, 45,0, 1},
								//{TURN, 0,0, 1},
								{DISPLACEMENT, 2850, 1800, 1},
								{TURN, 135, 0, 0},
								{TURN, 180, 0, 0},
								{DISPLACEMENT, 2300, 1800, 0},
								{DISPLACEMENT, 1937, 1800, 0}};




AtomeMontee::AtomeMontee() {
	trajectory_index = 0;
	time_start = 0;
	//flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = 80;
	angles.angleB = 100;
}

AtomeMontee::~AtomeMontee() {
	// TODO Auto-generated destructor stub
}

void AtomeMontee::enter() {

	//Serial.println("Etat premiere recalage");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_montee_purple[0][1],traj_montee_purple[0][2]);
	}
	else{
		navigator.move_to(traj_montee_yellow[0][1],traj_montee_yellow[0][2]);
	}
}

void AtomeMontee::leave() {

}

void AtomeMontee::doIt() {
	if(trajectory_index == 6){
		fsmSupervisor.setNextState(&deadState);
	}

	if(trajectory_index == 1){
			fsmSupervisor.setNextState(&deadState);
	}

	if(trajectory_index <= 5){
		angles = zone_observation(traj_montee_purple[trajectory_index][3],  traj_montee_purple[trajectory_index][0]);
	}
	if(trajectory_index == 5){
		mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
		mandibuleDroite.write(MANDIBULE_DROITE_BAS);
	}
	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;


		if(tiretteState.get_color() == PURPLE){
			trajectory_index += 1;
			if(traj_montee_purple[trajectory_index][0]==DISPLACEMENT){
				navigator.move_to(traj_montee_purple[trajectory_index][1],traj_montee_purple[trajectory_index][2]);
			}
			else if(traj_montee_purple[trajectory_index][0]==TURN){
				navigator.turn_to(traj_montee_purple[trajectory_index][1] );
			}

		}
		else{
			trajectory_index += 1;
			if(traj_montee_yellow[trajectory_index][0]==DISPLACEMENT){
				navigator.move_to(traj_montee_yellow[trajectory_index][1],traj_montee_yellow[trajectory_index][2]);
			}
			else if(traj_montee_yellow[trajectory_index][0]==TURN){
				navigator.turn_to(traj_montee_yellow[trajectory_index][1] );
			}
		}

	}
}

void AtomeMontee::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(traj_montee_purple[trajectory_index][1],traj_montee_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(traj_montee_yellow[trajectory_index][1],traj_montee_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}

}


void AtomeMontee::forceLeave(){

}

