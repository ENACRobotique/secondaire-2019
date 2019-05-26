/*
 * RangementChaos.cpp
 *
 *  Created on: 24 mai 2019
 *      Author: robot
 */


#include "04RecolteChaos.h"
#include "05RangementChaos.h"
#include "00TiretteState.h"
#include "06DeuxiemeRecalage.h"
#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"

RangementChaos rangementChaos = RangementChaos();


float traj_rangement2_purple[][3] = { {DISPLACEMENT,1000,400},
									{TURN,130,0},
									{TURN,170,0},
									{DISPLACEMENT,250,400},

};

float traj_rangement2_yellow[][2] = { {150,1200},
								{500,1200},
								{500,300}
};

RangementChaos::RangementChaos() {
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

RangementChaos::~RangementChaos() {
	// TODO Auto-generated destructor stub
}

void RangementChaos::enter() {

	//Serial.println("Etat premiere recolte");

	if(tiretteState.get_color() == PURPLE){
		navigator.move_to(traj_rangement2_purple[0][1],traj_rangement2_purple[0][2]);
	}
	else{
		navigator.move_to(traj_rangement2_yellow[0][1],traj_rangement2_yellow[0][2]);
		navigator.turn_to(-180); //TODO voir les bonnes valeurs pour le cotes jaunes
		navigator.move_to(traj_rangement2_yellow[1][0],traj_rangement2_yellow[1][1]);
	}

}

void RangementChaos::leave() {

}

void RangementChaos::doIt() {
	if(navigator.isTrajectoryFinished()){
		if(trajectory_index == 3){
			mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
			mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
			fsmSupervisor.setNextState(&deuxiemeRecalage);
		}
		else{
			if(tiretteState.get_color() == PURPLE){
				trajectory_index += 1;
				if(traj_rangement2_purple[trajectory_index][0]==DISPLACEMENT){
					navigator.move_to(traj_rangement2_purple[trajectory_index][1],traj_rangement2_purple[trajectory_index][2]);
				}
				else if(traj_rangement2_purple[trajectory_index][0]==TURN){
					navigator.turn_to(traj_rangement2_purple[trajectory_index][1] );
					if(trajectory_index == 2){
						Odometry::set_pos(1000, 400, 180);
					}
				}

			}
			else{
				//navigator.turn_to(turn_recalage1_yellow[trajectory_index]);
				trajectory_index += 1;
				navigator.move_to(traj_rangement2_yellow[trajectory_index][0],traj_rangement2_yellow[trajectory_index][1]);
			}
		}
	}
}

void RangementChaos::reEnter(unsigned long interruptTime){
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

void RangementChaos::forceLeave(){

}
