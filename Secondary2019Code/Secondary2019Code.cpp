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
Metro debugLed = Metro(500);

int PIN_LED = 13;
uint32_t deb;


//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(PIN_LED, OUTPUT);
	pinMode(MOT_LIDAR, OUTPUT);

	pinMode(COLOR,INPUT_PULLUP);
	pinMode(BATT_CHARGE, INPUT);

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


}

int i = 0;



// The loop function is called in an endless loop
void loop()
{
	fsmSupervisor.update();

	if(controlTime.check()) {
		Odometry::update();
		MotorControl::update();
	}

	if(navigatorTime.check()) {
		navigator.update();
	}
}
