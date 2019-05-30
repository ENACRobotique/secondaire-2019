/*
 // Do not remove the incl	supprimé :        Secondary2019Code/.cproject
	supprimé :        Secondary2019Code/.project
	supprimé :        Secondary2019Code/.settings/language.settings.xml
	supprimé :        Secondary2019Code/.settings/org.eclipse.cdt.codan.core.prefs
	supprimé :        Secondary2019Code/.settings/org.eclipse.cdt.core.prefs
	supprimé :        Secondary2019Code/spec.d
ude below	supprimé :        Secondary2019Code/.cproject
	supprimé :        Secondary2019Cod	supprimé :        Secondary2019Code/.cproject
	supprimé :        Secondary2019Code/.project
	supprimé :        Secondary2019Code/.settings/language.settings.xml
	supprimé :        Secondary2019Code/.settings/org.eclipse.cdt.codan.core.prefs
	supprimé :        Secondary2019Code/.settings/org.eclipse.cdt.core.prefs
	supprimé :        Secondary2019Code/spec.d
e/.project
	supprimé :        Secondary2019Code/.settings/language.settings.xml
	supprimé :        Secondary2019Code/.settings/org.eclipse.cdt.codan.core.prefs
	supprimé :        Secondary2019Code/.settings/org.eclipse.cdt.core.prefs
	supprimé :        Secondary2019Code/spec.d
*/
#include "Secondary2019Code.h"
#include "Metro.h"
#include "params.h"
#include "odometry.h"
#include "motorControl.h"
#include "Navigator.h"
#include "Servo.h"
#include "StateMachine/FSMSupervisor.h"
//#include "lidar/LidarXV11.h"
//#include "lidar/LidarManager.h"


#include "StateMachine/00TiretteState.h"

Metro controlTime = Metro((unsigned long)(CONTROL_PERIOD * 1000));
Metro navigatorTime = Metro(NAVIGATOR_TIME_PERIOD * 1000);
Metro lidarAsservTime = Metro(500);
Metro debugLed = Metro(500);

bool isLidarSet = false;
unsigned int lidarPWM = 25;

int PIN_LED = 13;
LidarManager lidarManager = LidarManager();
uint32_t deb;


//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(PIN_LED, OUTPUT);
	pinMode(MOT_LIDAR, OUTPUT);

	pinMode(COLOR,INPUT_PULLUP);
	pinMode(BATT_CHARGE, INPUT);


	int charge_batterie = analogRead(BATT_CHARGE);
	//analogWrite(MOT_LIDAR, 50/750 * charge_batterie);
	//analogWrite(MOT_LIDAR, 50);

	Serial1.begin(115200);
	Serial2.begin(115200);
	//while(!Serial){}
	Serial1.println("INIT Serial");
	Serial1.println(BATT_CHARGE);
	Odometry::init();
	MotorControl::init();
	fsmSupervisor.init(&tiretteState);
	controlTime.reset();
	navigatorTime.reset();
	deb = millis();

	mandibuleGauche.attach(SERVO2);
	mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
	mandibuleDroite.attach(SERVO1);
	mandibuleDroite.write(MANDIBULE_DROITE_HAUT);

	//Serial.println(lidarManager.is_ignored2(1254,1533));


}

int i = 0;



// The loop function is called in an endless loop
void loop()
{
	/*Serial1.print("BATT_CHARGE : ");
	Serial1.println(analogRead(BATT_CHARGE));*/
	//Serial1.print("COLOR   ");
	//Serial1.println(digitalRead(COLOR));
		lidarManager.update();
		//Serial1.println("HOLA");
		//Serial1.print("Distance :   ");




	fsmSupervisor.update();

	if(controlTime.check()) {
		Odometry::update();
		MotorControl::update();
	}

	if(navigatorTime.check()) {
		if (isLidarSet){
			Serial1.print("Distannnnnce : ");
			if (lidarManager.lidar.is_valid(270)){
				Serial1.println(lidarManager.lidar.get_distance(270));
			}else{
				Serial1.println("0");
			}
		}
		navigator.update();

	}

	if (!isLidarSet && lidarAsservTime.check()){
		Serial1.print("Speed:");
		Serial1.println(lidarManager.lidar.getSpeed());
		if (lidarManager.lidar.getSpeed() >= 260){
			lidarPWM -= 3;
			analogWrite(MOT_LIDAR, lidarPWM);
		}else if (lidarManager.lidar.getSpeed() <= 230){
			lidarPWM += 3;
			analogWrite(MOT_LIDAR, lidarPWM);
		}else{
			isLidarSet = true;
		}
	}


}
