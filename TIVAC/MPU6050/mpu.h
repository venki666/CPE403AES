/*
 * mpu.h
 *
 *  Created on: 18 Apr 2016
 *      Author: Kristjan
 */



void MPUtestConnection();
void initMPU6050();
void readMPU();
float getMPUangleY();
float getMPUangleX();
float getGyroX();
float getGyroY();
float getGyroZ();
int dataReadyMPU();
