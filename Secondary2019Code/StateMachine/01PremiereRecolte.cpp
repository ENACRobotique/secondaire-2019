/*
 * PremiereRecolte.cpp
 *
 *  Created on: 23 mai 2019
 *      Author: robot
 */


#include "00TiretteState.h"
#include "01PremiereRecolte.h"
#include "02RecolteChaos.h"

#include "../Navigator.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "../lib/USManager.h"
#include "../odometry.h"
#include "DeadState.h"

PremiereRecolte premiereRecolte = PremiereRecolte();


float parcourt[][4] = { {DISPLACEMENT,500,450, 2},     	// le premier champ dit si on tourne ou si l'on avance / recule, le deuxième est la position cible en x et le troisième en y,
						{TURN, 45,0, 0},				// le dernier donne des idnications sur le lidar : s'il vaut 0, le lidar est désactivé,sinon le numéro, indique quel angle on regarde avec le lidar.
						{TURN, 90,0, 0},
						{DISPLACEMENT,500,1100, 2},
						{DISPLACEMENT,500,550, 1},
						{TURN,135,0, 0},
						{TURN,180,0, 0},
						{DISPLACEMENT,200,550, 1}};


float parcourt_yellow[][4] = {  {DISPLACEMENT,2500, 450, 2},
								{TURN, 135,0, 0},
								{TURN, 90,0, 0},
								{DISPLACEMENT,2500,1100, 2},
								{DISPLACEMENT,2500,550, 1},
								{TURN, 45,0, 0},
								{TURN, 0,0, 0},
								{DISPLACEMENT,2800, 550, 1}};


PremiereRecolte::PremiereRecolte() {
	time_start = 0;
	trajectory_index = 0;
	flags = E_ULTRASOUND;
	usDistances.front_left = US_RANGE;  //changer selon côté de la table
	usDistances.front_right = US_RANGE;
	usDistances.rear_left = 0;
	usDistances.rear_right = 0;
	angles.angleA = lidar_av1;
	angles.angleB = lidar_av2;
}

PremiereRecolte::~PremiereRecolte() {
	// TODO Auto-generated destructor stub
}

void PremiereRecolte::enter() {
	// on fait le premier mouvement dans le enter, car le doIt commence par if(navigator.isTrajectoryFinished() or has_reentered), et que l'on reste donc bloquer  si on met tous dans le doIt
	has_reentered = 0;
	if (tiretteState.get_color() == PURPLE) {
		Serial1.println("On entre dans l'état 1 côté PURPLE ");
		navigator.move_to(parcourt[0][1],parcourt[0][2]);
	}
	else{
		Serial1.println("On entre dans l'état 1 YELLOW ");
		navigator.move_to(parcourt_yellow[0][1],parcourt_yellow[0][2]);
	}

}

void PremiereRecolte::leave() {
	//delay(250);
}

void PremiereRecolte::doIt() {// TODO Pour 2020, mettre le trajectory_index += 1; à la fin et pas au début, ça rendra le truc beaucoup plus naturel
	// on rentre dedans en boucle, donc il ne faut pas mettre 2 déplacements dans la même boucle, sinon on va juste ignorer le premier
	// on code les évènements spéciaux (bouger les pinces, faire un recalage) avec les if(trajectory_index == n)
	if(trajectory_index == 8){
					Serial.print("x  :  ");
					Serial.print(Odometry::get_pos_x());
					Serial.print("   y  :  ");
					Serial.println(Odometry::get_pos_y());
					fsmSupervisor.setNextState(&recolteChaos);
					//fsmSupervisor.setNextState(&deadState);
					return;
	}

	if(trajectory_index <= 7){//gère la zone d'observation du lidar
		angles = zone_observation(parcourt[trajectory_index][3],  parcourt[trajectory_index][0]);
	}

	if(trajectory_index == 4){
		mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
		mandibuleDroite.write(MANDIBULE_DROITE_BAS);
	}

	if(trajectory_index == 7){
			mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
			mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
		}


	if(navigator.isTrajectoryFinished() or has_reentered){
		has_reentered = 0;
		trajectory_index += 1;
		if (tiretteState.get_color() == PURPLE) {
			Serial.print("Entrée dans purple");
			if(parcourt[trajectory_index][0]==DISPLACEMENT  and trajectory_index <= 7){
				navigator.move_to(parcourt[trajectory_index][1],parcourt[trajectory_index][2]);
			}
			else if(parcourt[trajectory_index][0]==TURN){
				navigator.turn_to(parcourt[trajectory_index][1] );
			}
		}
		else{
				Serial.println("Entrée dans yellow");
				if(parcourt_yellow[trajectory_index][0]==DISPLACEMENT and trajectory_index <= 7){
					navigator.move_to(parcourt_yellow[trajectory_index][1],parcourt_yellow[trajectory_index][2]);
				}
				else if(parcourt_yellow[trajectory_index][0]==TURN){
					navigator.turn_to(parcourt_yellow[trajectory_index][1] );
				}
		}
	}
}

void PremiereRecolte::reEnter(unsigned long interruptTime){//
	time_start+=interruptTime;

	if(trajectory_index == 0){
		angles.angleA = lidar_av1;
		angles.angleB = lidar_av2;
		if(tiretteState.get_color() == PURPLE){
				navigator.move_to(parcourt[trajectory_index][1],parcourt[trajectory_index][2]);
		}
		else{
			navigator.move_to(parcourt_yellow[trajectory_index][1],parcourt_yellow[trajectory_index][2]);
		}
	}

	 if(trajectory_index >= 1){
		trajectory_index--;
		has_reentered = 1;
	}
}

void PremiereRecolte::forceLeave(){

}

