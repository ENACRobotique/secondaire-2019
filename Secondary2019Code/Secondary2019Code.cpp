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


#include "StateMachine/TiretteState.h"

Metro controlTime = Metro((unsigned long)(CONTROL_PERIOD * 1000));
Metro navigatorTime = Metro(NAVIGATOR_TIME_PERIOD * 1000);

//The setup function is called once at startup of the sketch

float speed = 200;
float omega = PI / 2;
int led = 13;
int deb;
//unsigned int temps[4] = {5000,500/speed*1000, 2000, 500/speed * 1000};
int xdep = 50;
int ydep = 420;
Servo mandibuleGauche = Servo();
Servo mandibuleDroite = Servo();

//LidarManager lidarManager = LidarManager();



void setup()
{
	pinMode(led, OUTPUT);
	pinMode(MOT_LIDAR, OUTPUT);
	analogWrite(MOT_LIDAR, 70);
	/*digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);               // wait for a second
	digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);*/
	Serial.begin(115200);
	Serial1.begin(115200);

	//while(!Serial){}
	Serial.println("INIT Serial");
	Odometry::init();
	//Serial.println("INIT odom");
	MotorControl::init();
	//Serial.println("INIT motor");
	fsmSupervisor.init(&tiretteState);
	controlTime.reset();
	navigatorTime.reset();
	deb = millis();
	Odometry::set_pos(xdep, ydep, 0);
	mandibuleGauche.attach(SERVO2);
	mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
	mandibuleDroite.attach(SERVO1);
	mandibuleDroite.write(MANDIBULE_DROITE_BAS);

	//Serial.println(lidarManager.is_ignored2(1254,1533));

}

int i = 0;



// The loop function is called in an endless loop
void loop()
{
	/*if (millis() - deb > 4000){
		mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
		mandibuleDroite.write(MANDIBULE_DROITE_BAS);
	}*/
/*
	if (Serial1.available()){
		lidarManager.update();
		//if (lidarManager.lidar.is_packet_available()){
		if(millis() - deb > 200){
			deb = millis();
			Odometry::set_pos(1500, 1300, 90);
			bool obs_detected = lidarManager.obstacleDetected(170, 190);
			Serial.println(obs_detected);
			//Serial.println(lidarManager.lidar.get_distance(180));

		}
	}

*/


/*
	if(i == 0){
			navigator.move_to(540, 420);
			i++;
	}
	if(navigator.isTrajectoryFinished() && i == 1){
			navigator.turn_to(90);
			i++;
	}
	if(navigator.isTrajectoryFinished() && i == 2){
			navigator.move_to(540, 1040);
			i++;
	}
	if(navigator.isTrajectoryFinished() && i == 3){
			navigator.turn_to(0);
			i++;
			mandibuleDroite.write(MANDIBULE_DROITE_BAS);
			mandibuleGauche.write(MANDIBULE_GAUCHE_BAS);
	}
	if(navigator.isTrajectoryFinished() && i == 4){
			navigator.move_to(110, 1040);
			i++;
	}
	if(navigator.isTrajectoryFinished() && i == 5){
			navigator.turn_to(90);
			i++;
		}
	if(navigator.isTrajectoryFinished() && i == 6){
			navigator.move_to(110,440);
			i++;

	}
	if(navigator.isTrajectoryFinished() && i == 7){
					navigator.move_to(110, -20);
					mandibuleDroite.write(MANDIBULE_DROITE_HAUT);
					mandibuleGauche.write(MANDIBULE_GAUCHE_HAUT);
					i++;
	}*/



	fsmSupervisor.update();

	if(controlTime.check()) {
		Odometry::update();
		MotorControl::update();
	}

	if(navigatorTime.check()) {
		navigator.update();

	}


}
