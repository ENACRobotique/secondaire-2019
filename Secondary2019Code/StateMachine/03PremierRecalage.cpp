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


float traj_recalage1_purple[][4] = { {DISPLACEMENT,650,550, 1},
									{TURN,135,0, 1},
									{TURN,90,0, 1},
									{DISPLACEMENT,650,780, 1},
									{TURN,45,0, 1},
									{TURN,0,0, 1},
									{DISPLACEMENT,-30,780, 0}
};


float traj_recalage1_yellow[][4	] = { {DISPLACEMENT,2350,550, 1},
									{TURN,45,0, 1},
									{TURN,90,0, 1},
									{DISPLACEMENT,2350,780, 1},
									{TURN,135,0, 1},
									{TURN,180,0, 1},
									{DISPLACEMENT,3030,780, 0}
};


float turn_recalage1_purple[] = { 90, 0 };

PremierRecalage::PremierRecalage() {
	trajectory_index = 0;
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = lidar_ar1;
	angles.angleB = lidar_ar2;
	//angles.angleA = 0;
	//angles.angleB = 0;
}

PremierRecalage::~PremierRecalage() {
	// TODO Auto-generated destructor stub
}

void PremierRecalage::enter() {
	has_reentered = 0;
	Serial.println("On entre dans enter de premierRecalage");
	//Odometry::set_pos(220, 550, 180);
	if (tiretteState.get_color() == PURPLE) {
		navigator.move_to(traj_recalage1_purple[trajectory_index][1], traj_recalage1_purple[trajectory_index][2]);
	}
	else{
		navigator.move_to(traj_recalage1_yellow[trajectory_index][1],traj_recalage1_yellow[trajectory_index][2]);
	}
	//Serial.println("Etat premiere recalage");

}

void PremierRecalage::leave() {

}

void PremierRecalage::doIt() {
	if(trajectory_index <= 6){
				angles = zone_observation(traj_recalage1_purple[trajectory_index][3],  traj_recalage1_purple[trajectory_index][0]);
	}
	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if (trajectory_index == 6) {
			if (tiretteState.get_color() == PURPLE) {
				Odometry::set_pos(25, 780, 0);
			} else {
					Odometry::set_pos(2975,780,180);
			}
			fsmSupervisor.setNextState(&recolteChaos);
			//fsmSupervisor.setNextState(&deadState);
		} 
		else {
			if (tiretteState.get_color() == PURPLE) {
				trajectory_index += 1;
				if (traj_recalage1_purple[trajectory_index][0] == DISPLACEMENT) {
					/*if (trajectory_index == 3) {
						angles.angleA = lidar_av1;
						angles.angleB = lidar_av2;
					}
					if (trajectory_index == 6) {
						angles.angleA = 0;
						angles.angleB = 0;
					}*/
					navigator.move_to(traj_recalage1_purple[trajectory_index][1], traj_recalage1_purple[trajectory_index][2]);
				}
				else if (traj_recalage1_purple[trajectory_index][0] == TURN) {
					//angles.angleA = 0;
					//angles.angleB = 0;
					navigator.turn_to(traj_recalage1_purple[trajectory_index][1]);
				}
			}
			else{
				trajectory_index += 1;
				if(traj_recalage1_yellow[trajectory_index][0]==DISPLACEMENT){
					/*if (trajectory_index == 3) {
						angles.angleA = lidar_av1;
						angles.angleB = lidar_av2;
					}
					if (trajectory_index == 6) {
						angles.angleA = 0;
						angles.angleB = 0;
					}*/
					navigator.move_to(traj_recalage1_yellow[trajectory_index][1],traj_recalage1_yellow[trajectory_index][2]);
				}
				else if(traj_recalage1_yellow[trajectory_index][0]==TURN){
					//angles.angleA = 0;
					//angles.angleB = 0;
					navigator.turn_to(traj_recalage1_yellow[trajectory_index][1] );
				}
			}

		}
	}
}

void PremierRecalage::reEnter(unsigned long interruptTime) {
	time_start += interruptTime;

	if (trajectory_index == 0) {
		angles.angleA = lidar_ar1;
		angles.angleB = lidar_ar2;
		if (tiretteState.get_color() == PURPLE) {
			navigator.move_to(traj_recalage1_purple[trajectory_index][1], traj_recalage1_purple[trajectory_index][2]);
		} else {
			navigator.move_to(traj_recalage1_yellow[trajectory_index][1],
					traj_recalage1_yellow[trajectory_index][2]);
		}
	}

	else if (trajectory_index >= 1) {
		trajectory_index--;
		has_reentered = 1;
	}
}

void PremierRecalage::forceLeave() {

}

