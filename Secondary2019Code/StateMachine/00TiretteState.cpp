/*
 * TiretteState.cpp
 *
 *  Created on: 11 avr. 2018
 *      Author: robot
 */

#include "00TiretteState.h"
#include "TurnToButtonState.h"
#include "MoveToWaterState.h"
#include "01PremiereRecolte.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "DynamixelSerial4.h"
#include "../odometry.h"
//#include "../libraries/i2c_t3/i2c_t3.h"
#include "../lib/USManager.h"

TiretteState tiretteState = TiretteState();
Servo arm = Servo();

Servo mandibuleGauche = Servo();
Servo mandibuleDroite = Servo();


unsigned long time_us = 0;
TiretteState::TiretteState() {
	time_start = 0;
	flags = E_ULTRASOUND;
	COLOR_BEGIN = 1;
}

TiretteState::~TiretteState() {
	// TODO Auto-generated destructor stub
}

void TiretteState::enter() {
	Serial.println("Entrer dans tiretteState");
	time_start = millis();
	Serial.println("Etat tirette");


	pinMode(TIRETTE,INPUT_PULLUP);
	pinMode(COLOR,INPUT_PULLUP);
  //  Wire2.begin(I2C_MASTER, 0x00, I2C_PINS_3_4, I2C_PULLUP_EXT, 400000);
    //TODO FIx this shit
	//Wire2.setSDA(4);
	//Wire2.setSCL(3);
	uint8_t USadresses[] = {0X75, 0X71, 0X70, 0X73};
	usManager.init(USadresses);
}

void TiretteState::leave() {
	if(digitalRead(COLOR) == PURPLE){
		Odometry::set_pos(150, 650, 90);
		COLOR_BEGIN = PURPLE;
	}
	else{
		Odometry::set_pos(100, 300, 0);
		COLOR_BEGIN = YELLOW;
	}
	Odometry::set_pos(150, 650, 90);
	COLOR_BEGIN = PURPLE;
}

void TiretteState::doIt() {
	time_start = millis();
	if (!digitalRead(TIRETTE)) {
		Serial.println("On change d'etat : gooooo!!");
		time_start = millis();
		fsmSupervisor.setNextState(&premiereRecolte);
	}

}

void TiretteState::reEnter(unsigned long interruptTime){

}

void TiretteState::forceLeave(){

}

int TiretteState::get_color(){
	return COLOR_BEGIN;
}