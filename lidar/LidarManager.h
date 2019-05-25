/*
 * LidarManager.cpp
 *
 *  Created on: 14 mai 2019
 *      Author: robot
 */

#include "LidarXV11.h"
#include "Arduino.h"

class LidarManager {
public:
	LidarManager();
	virtual ~LidarManager();
	void init();


private:
	LidarXV11 lidar;
	int xObstacle(int lidarAngle);
	int yObstacle(int lidarAngle);

};
