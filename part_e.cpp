#include "Aria.h"
#include <iostream>
#include <math.h>

using namespace std;

int main (int argc, char **argv) {
	ArRobot robot;
	ArSonarDevice sonar;

	double x, y, th;
    cin >> x >> y >> th;

	robot.addRangeDevice(&sonar);

	Aria::init();

	ArSimpleConnector connector(&argc,argv);

	if (!connector.connectRobot(&robot)){
		printf("Could not connect to robot... exiting\n");
		Aria::shutdown();
		Aria::exit(1);
	}

	robot.comInt(ArCommands::ENABLE, 1);

	robot.runAsync(false);

  // Used to perform actions when keyboard keys are pressed
  ArKeyHandler keyHandler;
  Aria::setKeyHandler(&keyHandler);

  // ArRobot contains an exit action for the Escape key. It also
  // stores a pointer to the keyhandler so that other parts of the program can
  // use the same keyhandler.
  robot.attachKeyHandler(&keyHandler);
  printf("You may press escape to exit\n");

	// 1. Lock the robot
	robot.lock();

	double heading = atan(y/x);
	double distance = sqrt(pow(x, 2) + pow(y, 2));
	bool started = false;

	robot.setHeading (180*heading/M_PI);

	// 3. Unlock the robot
	robot.unlock();

	cout << robot.getX() << " " << robot.getY() << " " << robot.getTh() << endl;

	// 4. Sleep a while and let the robot move
	while (true) {
		//printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
		if (robot.isHeadingDone () && !started) {
			robot.resetTripOdometer ();
			robot.setVel (distance/2);
			started = true;
		}
		if (started && distance - robot.getTripOdometerDistance () < robot.getVel ()) {
			robot.setVel (0);
			break;
		}
	}

	robot.setHeading (th*180/M_PI);
	while (!robot.isHeadingDone ()) {}

	cout << robot.getX() << " " << robot.getY() << " " << robot.getTh();

	// End of controling


	Aria::shutdown();

	Aria::exit(0);
}
