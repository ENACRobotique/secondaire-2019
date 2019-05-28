/*
 * TiretteState.cpp
 *
 *  Created on: 11 avr. 2018
 *      Author: robot
 */

#include "00TiretteState.h"
#include "01PremiereRecolte.h"
#include "01PremiereRecoltey.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "Servo.h"
#include "../odometry.h"
#include "../libraries/i2c_t3/i2c_t3.h"
#include "../lib/USManager.h"
#include "03PremierRecalage.h"


TiretteState tiretteState = TiretteState();
Servo arm = Servo();

Servo mandibuleGauche = Servo();
Servo mandibuleDroite = Servo();


unsigned long time_us = 0;
TiretteState::TiretteState() {
	time_start = 0;
	flags = E_ULTRASOUND;
	COLOR_BEGIN = PURPLE;
	angles.angleA = 80;
	angles.angleB = 100;
}

TiretteState::~TiretteState() {
	// TODO Auto-generated destructor stub
}

void TiretteState::enter() {
	Serial.println("Entrer dans tiretteState");
	time_start = millis();
	Serial.println("Etat tirette");

	int charge_batterie = analogRead(BATT_CHARGE);
	//analogWrite(MOT_LIDAR, 50/825 * charge_batterie);
	analogWrite(MOT_LIDAR, 55);
	Serial2.print("charge  ");
	Serial2.println(charge_batterie);
	//analogWrite(MOT_LIDAR, (int)((50./825.) * charge_batterie));
	Serial2.println((float)(50./825.) * charge_batterie);
	pinMode(TIRETTE,INPUT_PULLUP);
	pinMode(COLOR,INPUT_PULLUP);
    /*Wire2.begin(I2C_MASTER, 0x00, I2C_PINS_3_4, I2C_PULLUP_EXT, 400000);
    //TODO FIx this shit
	Wire2.setSDA(4);
	Wire2.setSCL(3);
	uint8_t USadresses[] = {0X78, 0X74, 0X70, 0X73};
	usManager.init(USadresses);*/
	//COLOR_BEGIN = PURPLE;
	//COLOR_BEGIN = YELLOW;

}

void TiretteState::leave() {
	Serial2.print("COLOR  ");
	Serial2.println(digitalRead(COLOR));
	Serial2.print("Color : ");
	Serial2.println(digitalRead(COLOR) == PURPLE);
	if(digitalRead(COLOR) == PURPLE){
		Odometry::set_pos(150, 650, 90);
		COLOR_BEGIN = PURPLE;
	}
	else{
		Serial2.println("GO YELLOW");
		Odometry::set_pos(2850, 650, 90);
		COLOR_BEGIN = YELLOW;
	}
	/*Odometry::set_pos(150, 650, 90);
	COLOR_BEGIN = PURPLE;
	Odometry::set_pos(2850, 650, 90);
	COLOR_BEGIN = YELLOW;*/
}

void TiretteState::doIt() {
	time_start = millis();
	Serial.println("On entre dans doIt!");
	Serial.print("color : ");
	Serial.println(COLOR_BEGIN);

	if(digitalRead(COLOR) == PURPLE){
		Odometry::set_pos(150, 650, 90);
		COLOR_BEGIN = PURPLE;
	}
	else{
		//Serial2.println("GO YELLOW");
		Odometry::set_pos(2850, 650, 90);
		COLOR_BEGIN = YELLOW;
	}

	if (!digitalRead(TIRETTE)) {
		Serial.println("On change d'etat : gooooo!!");
		time_start = millis();
		if(COLOR_BEGIN == PURPLE){
			Serial.println("On change d'etat : PURPLE!!");
			fsmSupervisor.setNextState(&premiereRecolte);
		}
		else{
			Serial.println("On change d'etat : YELLOW!!");
			fsmSupervisor.setNextState(&premiereRecoltey);
		}
	}

}

void TiretteState::reEnter(unsigned long interruptTime){

}

void TiretteState::forceLeave(){

}

int TiretteState::get_color(){
	return COLOR_BEGIN;
}
