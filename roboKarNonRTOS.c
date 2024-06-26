/*
 *   ROBOSAMPLE.C -- A sample/template for RoboKar program with uCOS-II
 *   Written by: Rosbi Mamat 6/5/2014
 *   Updated : 23/6/2024 Modified to show proximity & light sensor usage
 	 Updated by : Team Cheezylicious
 */

#include "..\inc\hal_robo.h"  /* RoboKar HAL */

#define STOP_SPEED 0

/* ------ Global shared variable -------*/
struct robostate
{
    int rspeed;     /* right motor speed  (-100 -- +100) */
    int lspeed;     /* left motor speed  (-100 -- +100) */
    char obstacle;  /* obstacle? 1 = yes, 0 = no */
} myrobot;

void CheckCollision(void)
{
    if (robo_proxSensor() == 1) {    /* obstacle? */
        myrobot.obstacle = 1;        /* signal obstacle present */
    } else {
        myrobot.obstacle = 0;        /* signal no obstacle */
    }
}

void CntrlMotors(void)
{
    robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
}

void Navig(void)
{
    if (myrobot.obstacle == 0)
    {
        if (robo_lightSensor() > 90) /* when light shines, we move forward */
        {
            myrobot.rspeed = 80;    /* move forward */
            myrobot.lspeed = 80;
        }
        // turn right slow
        if (robo_lineSensor() == 1) {
            myrobot.rspeed = 50;
            myrobot.lspeed = 80;
        // turn right fast
        } else if (robo_lineSensor() == 3) {
            myrobot.rspeed = -50;
            myrobot.lspeed = 80;
        // Forward fast
        } else if (robo_lineSensor() == 2) {
            myrobot.rspeed = 80;
            myrobot.lspeed = 80;
        // turn left fast
        } else if (robo_lineSensor() == 6) {
            myrobot.rspeed = 80;
            myrobot.lspeed = -60;
        // turn left slow
        } else if (robo_lineSensor() == 4) {
            myrobot.rspeed = 80;
            myrobot.lspeed = -20;
        } else if (robo_lineSensor() == 5) {
            myrobot.rspeed = 80;
            myrobot.lspeed = 80;
        } else if (robo_lineSensor() == 7) {
            myrobot.rspeed = 80;
            myrobot.lspeed = 80;
        // STOP
        } else if (robo_lineSensor() == 0) {
            myrobot.rspeed = -50;
            myrobot.lspeed = -35;
        }
    } else if (myrobot.obstacle == 1 && robo_lineSensor() == 0) { // Obstacle
        myrobot.rspeed = 0; // stop
        myrobot.lspeed = 0;
        CntrlMotors();
        robo_wait(600);

        myrobot.rspeed = -50; // turn to right
        myrobot.lspeed = 80;
        CntrlMotors();
        robo_wait(200);

        myrobot.rspeed = 50; // move straight
        myrobot.lspeed = 50;
        CntrlMotors();
        robo_wait(2000);
    } else if (myrobot.obstacle == 1 && (robo_lineSensor() >= 1 && robo_lineSensor() <= 7)) { // Roundabout
        myrobot.rspeed = 0; // stop
        myrobot.lspeed = 0;
        CntrlMotors();
        robo_wait(150);
        
        int lineSensor = robo_lineSensor();
        // Handle line sensors
        if (lineSensor == 1) {
            myrobot.rspeed = 50;
            myrobot.lspeed = 80;
        } else if (lineSensor == 3) {
            myrobot.rspeed = -50;
            myrobot.lspeed = 80;
        } else if (lineSensor == 2) {
            myrobot.rspeed = 80;
            myrobot.lspeed = 80;
        } else if (lineSensor == 6) {
            myrobot.rspeed = 80;
            myrobot.lspeed = -60;
        } else if (lineSensor == 4) {
            myrobot.rspeed = 80;
            myrobot.lspeed = -20;
        } else if (lineSensor == 5) {
            myrobot.rspeed = 80;
            myrobot.lspeed = 80;
        } else if (lineSensor == 7) {
            myrobot.rspeed = 80;
            myrobot.lspeed = 80;
        } else if (lineSensor == 0) {
            myrobot.rspeed = -50;
            myrobot.lspeed = -35;
        }
    }
}

/* Simple delay function using busy-wait */
void robo_wait(unsigned int ms)
{
    volatile unsigned long count;
    while (ms > 0)
    {
        count = 1000;  // Adjust this count to achieve approximately 1 millisecond delay on your hardware
        while (count > 0)
        {
            count--;
        }
        ms--;
    }
}

int main(void)
{
    robo_Setup();                          /* initialize HAL for RoboKar */
    robo_motorSpeed(STOP_SPEED, STOP_SPEED); /* Stop the robot */
    myrobot.rspeed = STOP_SPEED;           /* Initialize myrobot states */
    myrobot.lspeed = STOP_SPEED;
    myrobot.obstacle = 0;                  /* No collision */

    robo_Honk();
    robo_wait4goPress();                   /* Wait for to GO */

    while (1)
    {
        CheckCollision();
        Navig();
        CntrlMotors();
        robo_wait(5);                      /* Main loop period ~ 5 ms */
    }

    return 0;
}

