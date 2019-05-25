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

int PIN_LED = 13;
LidarManager lidarManager = LidarManager();
uint32_t deb;


//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(PIN_LED, OUTPUT);
	pinMode(MOT_LIDAR, OUTPUT);
	analogWrite(MOT_LIDAR, 70);

	Serial.begin(115200);
	Serial1.begin(115200);
	//while(!Serial){}
	Serial.println("INIT Serial");

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
	if (Serial1.available()){
		lidarManager.update();
		if (lidarManager.lidar.is_packet_available()){
		//if(millis() - deb > 200){
			deb = millis();
			//Odometry::set_pos(1500, 1300, 90);
			//bool obs_detected = lidarManager.obstacleDetected(170, 190);
			//Serial.println(obs_detected);
			//Serial.println(lidarManager.lidar.get_distance(180));

		}
	}


	fsmSupervisor.update();

	if(controlTime.check()) {
		Odometry::update();
		MotorControl::update();
	}

	if(navigatorTime.check()) {
		navigator.update();

	}


}
