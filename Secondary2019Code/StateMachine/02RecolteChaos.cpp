/*
 * RecolteChaos.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "00TiretteState.h"
#include "02RecolteChaos.h"
#include "03RangementChaos.h"

#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "DeadState.h"

RecolteChaos recolteChaos = RecolteChaos();


float traj_recolte2_purple[][4] = { {DISPLACEMENT,1000,700, 1},
									{TURN,45,0, 0},
									{TURN,90,0, 0},
									{DISPLACEMENT,1000,1150, 1},

};


float traj_recolte2_yellow[][4] = { {DISPLACEMENT,2000,700, 1},
									{TURN,135,0, 0},
									{TURN,90,0, 0},
									{DISPLACEMENT,2000,1150, 1},

};


RecolteChaos::RecolteChaos() {
	trajectory_index = 0;
	time_start = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = 0;
	usDistances.front_right = 0;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = lidar_av1;
	angles.angleB = lidar_av2;
}

RecolteChaos::~RecolteChaos() {
	// TODO Auto-generated destructor stub
}

void RecolteChaos::enter() {
	has_reentered = 0;
	Serial1.println("Etat 2 : Recolte chaos");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_recolte2_purple[0][1],traj_recolte2_purple[0][2]);
	}
	else{
		navigator.move_to(traj_recolte2_yellow[0][1],traj_recolte2_yellow[0][2]);
	}
}

void RecolteChaos::leave() {

}

void RecolteChaos::doIt() {
	if(trajectory_index <= 3){
		angles = zone_observation(traj_recolte2_purple[trajectory_index][3],  traj_recolte2_purple[trajectory_index][0]);
	}
	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		if(trajectory_index == 3){
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
			delay(250);
			fsmSupervisor.setNextState(&rangementChaos);
			//fsmSupervisor.setNextState(&deadState);

		}
		else{
			if(tiretteState.get_color() == PURPLE){
				trajectory_index += 1;
				if(traj_recolte2_purple[trajectory_index][0]==DISPLACEMENT){
					/*if(trajectory_index == 3){
						angles.angleA = lidar_av1;
						angles.angleB = lidar_av2;
					}*/
					navigator.move_to(traj_recolte2_purple[trajectory_index][1],traj_recolte2_purple[trajectory_index][2]);
				}
				else if(traj_recolte2_purple[trajectory_index][0]==TURN){
					/*angles.angleA = 0;
					angles.angleB = 0;*/
					navigator.turn_to(traj_recolte2_purple[trajectory_index][1] );
				}

			}
			else{
				trajectory_index += 1;
				if(traj_recolte2_yellow[trajectory_index][0]==DISPLACEMENT){
					/*if(trajectory_index == 3){
						angles.angleA = lidar_av1;
						angles.angleB = lidar_av2;
					}*/
					navigator.move_to(traj_recolte2_yellow[trajectory_index][1],traj_recolte2_yellow[trajectory_index][2]);
				}
				else if(traj_recolte2_yellow[trajectory_index][0]==TURN){
					/*angles.angleA = 0;
					angles.angleB = 0;*/
					navigator.turn_to(traj_recolte2_yellow[trajectory_index][1]);
				}
			}
		}
	}
}

void RecolteChaos::reEnter(unsigned long interruptTime){
	time_start+=interruptTime;
	if(trajectory_index == 0){
		angles.angleA = lidar_av1;
		angles.angleB = lidar_av2;
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(traj_recolte2_purple[trajectory_index][1],traj_recolte2_purple[trajectory_index][2]);
		}
		else{
			navigator.move_to(traj_recolte2_yellow[trajectory_index][1],traj_recolte2_yellow[trajectory_index][2]);
		}
	}

	else if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void RecolteChaos::forceLeave(){

}


