#include <iostream>		// pulls in cin and hex
#include <cstdio>		// pulls in printf()
#include <cstdlib>		// lets me use system()

#include "LSM9DS0.h"

using namespace std;

// Functions to put things on the screen.
void printMenu() {
	printf("\n\t\t-----MAIN MENU-----");
	printf("\n\t 1) Print Main Menu");
	printf("\n\t 2) Read Accel");
	printf("\n\t 3) Read Mag");
	printf("\n\t 4) Read Gyro");
	printf("\n\t 5) Read Temp");
	printf("\n\t 6) Read All");
	printf("\n\t 7) Read Many");
	printf("\n\t 0) Quit");
	printf("\nInput selection ");
}

// Functions to get inputs
void getHexInput(unsigned int *ptr) {
	for (;;) {
		if (cin >> hex >> *ptr) {
			return;
		} else {
			printf("INVALID INPUT.");
			printf("\nInput a hex number: ");
			cin.clear();
			cin.ignore(80, '\n');
		}
	}
}

void getDecInput(unsigned int *ptr) {
	for (;;) {
		if (cin >> dec >> *ptr) {
			return;
		} else {
			printf("INVALID INPUT.");
			printf("\nInput a dec number: ");
			cin.clear();
			cin.ignore(80, '\n');
		}
	}
}

int main(int argc, char* argv[]) {
	
	PatricksDrivers::LSM9DS0 SensorBoard(
		1,
		PatricksDrivers::LSM9DS0_ACCELRANGE_2G,
		PatricksDrivers::LSM9DS0_MAGGAIN_2GAUSS,
		PatricksDrivers::LSM9DS0_GYROSCALE_245DPS
	);
	
	unsigned int menu_choice;
	
	system("clear");
	
	do {
		
		printMenu();
		
		getDecInput(&menu_choice);
		
		switch (menu_choice) {
			case 1:
				printMenu();
			break;
			
			case 2:
				SensorBoard.readAccel();
				system("clear");
				printf("X = %f\tY = %f\tZ = %f", SensorBoard.acceleration.x, SensorBoard.acceleration.y, SensorBoard.acceleration.z);
			break;
			
			case 3:
				SensorBoard.readMag();
				system("clear");
				printf("X = %f\tY = %f\tZ = %f", SensorBoard.magnetic.x, SensorBoard.magnetic.y, SensorBoard.magnetic.z);
			break;
			
			case 4:
				SensorBoard.readGyro();
				system("clear");
				printf("X = %f\tY = %f\tZ = %f", SensorBoard.gyroscopic.x, SensorBoard.gyroscopic.y, SensorBoard.gyroscopic.z);
			break;
			
			case 5:
				SensorBoard.readTemp();
				system("clear");
				printf("Temp = %f", SensorBoard.temperature);
			break;
			
			case 6:
				SensorBoard.readAccel();
				SensorBoard.readMag();
				SensorBoard.readGyro();
				SensorBoard.readTemp();
				system("clear");
				printf("Accel:\tX = %f\tY = %f\tZ = %f", SensorBoard.acceleration.x, SensorBoard.acceleration.y, SensorBoard.acceleration.z);
				printf("\nMag\tX = %f\tY = %f\tZ = %f", SensorBoard.magnetic.x, SensorBoard.magnetic.y, SensorBoard.magnetic.z);
				printf("\nGyro\tX = %f\tY = %f\tZ = %f", SensorBoard.gyroscopic.x, SensorBoard.gyroscopic.y, SensorBoard.gyroscopic.z);
				printf("\nTemp = %f", SensorBoard.temperature);
			break;
			
			case 7:
				for (int i = 0; i < 1000; i++) {
					SensorBoard.readAccel();
					SensorBoard.readMag();
					SensorBoard.readGyro();
					SensorBoard.readTemp();
					system("clear");
					usleep(250);
					printf("\nAccel:\tX = %f\tY = %f\tZ = %f", SensorBoard.acceleration.x, SensorBoard.acceleration.y, SensorBoard.acceleration.z);
					printf("\nMag\tX = %f\tY = %f\tZ = %f", SensorBoard.magnetic.x, SensorBoard.magnetic.y, SensorBoard.magnetic.z);
					printf("\nGyro\tX = %f\tY = %f\tZ = %f", SensorBoard.gyroscopic.x, SensorBoard.gyroscopic.y, SensorBoard.gyroscopic.z);
					printf("\nTemp = %f", SensorBoard.temperature);
					printf("\ni = %i", i);
				}
			break;
			
			case 0:
				; // do nothing
			break;
			
			default:
				printf("\nINVALID SELECTION.");
			
		}
		
	} while (menu_choice != 0);
	
	printf("Goodbye!\n");
	
	return 0;
}
