/*
 * LidarManager.cpp
 *
 *  Created on: 14 mai 2019
 *      Author: robot
 */

#include "LidarManager.h"
#include "LidarXV11.h"
#include "Arduino.h"
#include "../odometry.h"
#include "map.h"


//LidarManager lidarManager = LidarManager();

LidarManager::LidarManager() {

}

LidarManager::~LidarManager() {
}

void LidarManager::init() {
}

void LidarManager::update() {
	uint8_t b = Serial1.read();
	lidar.update(b);
}

int LidarManager::xObstacle(int lidarAngle) {
	int d =  lidar.get_distance(lidarAngle);
	int robotAngle = Odometry::get_pos_theta();
	int x = Odometry::get_pos_x();
	int xOb =  x + d * cos(lidarAngle * DEG_TO_RAD + robotAngle);
	return(xOb);

}

int LidarManager::yObstacle(int lidarAngle) {
	int d =  lidar.get_distance(lidarAngle);
	int robotAngle = Odometry::get_pos_theta();
	int y = Odometry::get_pos_y();
	int yOb =  y + d * sin(lidarAngle * DEG_TO_RAD + robotAngle);
	return(yOb);
}

bool LidarManager::is_ignored(int lidarAngle){
	int x = LidarManager::xObstacle(lidarAngle);
	int y = LidarManager::yObstacle(lidarAngle);
//	Serial.print(x);
//	Serial.print("   ");
//	Serial.println(y);
	bool ignored = false;
	if (x < 0 or x >= Map::table_size_x or y < 0 or y >= Map::table_size_y){
		ignored = true;

	}
	else if ((Map::obstacle_map[(int)(y * Map::map_pos_y / Map::table_size_y)][(int)(x * Map::map_pos_x / Map::table_size_x)]) & (0b1 << (7 - (int)(x / Map::PAS) % 8)) ){ // comparaison de carte
		ignored = true;
		digitalWrite(13, HIGH);

	}

	Serial.println(ignored);

	return(ignored);
}

bool LidarManager::is_ignored2(int x, int y){
	bool ignored = false;
	if (x < 0 or x >= Map::table_size_x or y < 0 or y >= Map::table_size_y){
		ignored = true;
	}
	else if ((Map::obstacle_map[(int)(y * Map::map_pos_y / Map::table_size_y)][(int)(x * Map::map_pos_x / Map::table_size_x)]) & (0b1 << (7 - (int)(x / Map::PAS) % 8)) ){ // comparaison de carte
		ignored = true;
	}
	return(ignored);
}

bool LidarManager::obstacleDetected(int lidarAngleA, int lidarAngleB){
	bool obstacle = false;
	int i = 0;
	for(int angle = lidarAngleA; angle <= lidarAngleB; angle++){
		//Serial.print("distance   ");
		//Serial.println(lidar.get_distance(angle));
		if(lidar.is_valid(angle) and lidar.get_distance(angle) < DIST_MIN){
			int x= LidarManager::xObstacle(angle);
			int y = LidarManager::yObstacle(angle);

			 if(!LidarManager::is_ignored(angle)){
						i++;
						/*Serial.print(x);
						Serial.print("    ");
						Serial.println(y);*/

			}
		}

	}
	if(i >= NB_OBSTACLES){
		obstacle = true;
	}

	return(obstacle);
}

