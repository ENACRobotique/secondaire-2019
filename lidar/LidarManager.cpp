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



LidarManager lidarManager = LidarManager();

LidarManager::LidarManager() {

}

LidarManager::~LidarManager() {
}

void LidarManager::init() {
	lidar = new LidarXV11();
}

int LidarManager::xObstacle(int lidarAngle) {
	int d =  lidar.distance_at_angle(lidarAngle);
	int robotAngle = Odometry::get_pos_theta();
	int x = Odometry::get_pos_x();
	int xOb =  x + d * cos(lidarAngle + robotAngle);

	return xOb;
}

int LidarManager::yObstacle(int lidarAngle) {
	int d =  lidar.distance_at_angle(lidarAngle);
	int robotAngle = Odometry::get_pos_theta();
	int y = Odometry::get_pos_y();
	int yOb =  y + d * cos(lidarAngle + robotAngle);

	return yOb;
}
