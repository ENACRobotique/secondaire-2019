/*
 * TiretteState.cpp
 *
 *  Created on: 11 avr. 2018
 *      Author: robot
 */


#include "00TiretteState.h"
#include "01PremiereRecolte.h"
#include "Arduino.h"
#include "../params.h"
#include "FSMSupervisor.h"
#include "Servo.h"
#include "../odometry.h"
#include "../libraries/i2c_t3/i2c_t3.h"
#include "../lib/USManager.h"
#include "05Balance.h"


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
	time_color = millis();
	Serial.println("Etat tirette");

	int charge_batterie = analogRead(BATT_CHARGE);
	//analogWrite(MOT_LIDAR, 50/825 * charge_batterie);
	//analogWrite(MOT_LIDAR, 55);
	Serial1.print("charge  ");
	Serial1.println(charge_batterie);
	//analogWrite(MOT_LIDAR, (int)((50./750.) * charge_batterie));
	//analogWrite(MOT_LIDAR, 50);
	//Serial1.println((float)(50./825.) * charge_batterie);
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
	Serial1.print("COLOR  ");
	Serial1.println(digitalRead(COLOR));
	Serial1.print("Color : ");
	Serial1.println(digitalRead(COLOR) == PURPLE);
	if(digitalRead(COLOR) == PURPLE){
		//Odometry::set_pos(245, 450, 0);
		Odometry::set_pos(50, 450, 0);
		//Odometry::set_pos(350, 700, -90);
		COLOR_BEGIN = PURPLE;
	}
	else{
		Odometry::set_pos(2755, 450, 180);
		//Odometry::set_pos(2650, 700, -90);
		COLOR_BEGIN = YELLOW;
	}
}

void TiretteState::doIt() {
	time_start = millis();


	if(digitalRead(COLOR) == PURPLE){
		//Serial1.println("PURPLE");
		if(millis() - time_color > 1000){
			digitalWrite(13, LOW);
			time_color = millis();
		}
		else if(millis() - time_color > 500){
			digitalWrite(13, HIGH);
		}
	}
	else{
		//Serial1.println("GO YELLOW");
		if(millis() - time_color > 100){
			digitalWrite(13, LOW);
			time_color = millis();
		}
		else if(millis() - time_color > 50){
			digitalWrite(13, HIGH);
		}
	}

	if (!digitalRead(TIRETTE)) {
		Serial1.println("On change d'etat : gooooo!!");
		time_start = millis();
		fsmSupervisor.setNextState(&premiereRecolte);
		//fsmSupervisor.setNextState(&balance);
	}

}

void TiretteState::reEnter(unsigned long interruptTime){

}

void TiretteState::forceLeave(){

}

int TiretteState::get_color(){
	return COLOR_BEGIN;
}
