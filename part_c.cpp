#include "Aria.h"

ArRobot robot;
ArSonarDevice sonar;

int rotVel, vel, accSpeed = 200, rotSpeed = 10;

void turnLeft () {
    rotVel += rotSpeed;
    if (rotVel >= robot.getAbsoluteMaxRotVel ())
        rotVel = robot.getAbsoluteMaxRotVel ();
    robot.setRotVel(rotVel);
}

void turnRight () {
    rotVel -= rotSpeed;
    if (rotVel <= -robot.getAbsoluteMaxRotVel ())
        rotVel = -robot.getAbsoluteMaxRotVel ();
    robot.setRotVel(rotVel);
}

void accelerate () {
    vel += accSpeed;
    if (vel >= robot.getAbsoluteMaxTransVel ())
        vel = robot.getAbsoluteMaxTransVel ();
    robot.setVel(vel);
}

void decelerate () {
    vel -= accSpeed;
    if (vel <= robot.getAbsoluteMaxTransNegVel ())
        vel = robot.getAbsoluteMaxTransNegVel ();
    robot.setVel(vel);
}

void breakVelocity (int* vel) {
    if (*vel > 0)
        *vel -= 1;
    else
        *vel += 1;
    if (*vel <= -1 || *vel >= 1)
        *vel = 0;
}


int main (int argc, char** argv) {

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
    ArGlobalFunctor left(&turnLeft);
    ArGlobalFunctor right(&turnRight);
    ArGlobalFunctor up(&accelerate);
    ArGlobalFunctor down(&decelerate);
    keyHandler.addKeyHandler(ArKeyHandler::LEFT, &left);
    keyHandler.addKeyHandler(ArKeyHandler::RIGHT, &right);
    keyHandler.addKeyHandler(ArKeyHandler::UP, &up);
    keyHandler.addKeyHandler(ArKeyHandler::DOWN, &down);
    Aria::setKeyHandler(&keyHandler);

    // ArRobot contains an exit action for the Escape key. It also
    // stores a pointer to the keyhandler so that other parts of the program can
    // use the same keyhandler.
    robot.attachKeyHandler(&keyHandler);
    printf("You may press escape to exit\n");

    // TODO: control the robot

    // Start of controling

    // 1. Lock the robot
    robot.lock();

    // 2. Write your control code here,
    //robot.setVel(150);


    // 3. Unlock the robot
    robot.unlock();

    // 4. Sleep a while and let the robot move
    while(true){
    	//printf("%f %f %f\n", robot.getX(), robot.getY(), robot.getTh());
    	//ArUtil::sleep(300);
        breakVelocity (&rotVel);
        robot.setRotVel (rotVel);
        breakVelocity (&vel);
        robot.setVel (vel);
    }

    // End of controling

    Aria::shutdown();

    Aria::exit(0);

    return 0;
}
