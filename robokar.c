/*
 *   ROBOSAMPLE.C -- A sample/template for RoboKar program with uCOS-II
 *   Written by: Rosbi Mamat 6/5/2014
 *   Updated : 1/5/2023 Modified to show proximity & light sensor usage
 */

#include "..\inc\kernel.h"                  /* Always include these to use uCOS-II      */
#include "..\inc\hal_robo.h"                /*   and RoboKar HAL                        */

#define TASK_STK_SZ            128          /* Size of each task's stacks (# of bytes)  */
#define TASK_START_PRIO          1          /* Highest priority                         */
#define TASK_CHKCOLLIDE_PRIO     2
#define TASK_CTRLMOTOR_PRIO      3
#define TASK_NAVIG_PRIO          4          /* Lowest priority                          */

OS_STK TaskStartStk[TASK_STK_SZ];           /* TaskStartTask stack                      */
OS_STK ChkCollideStk[TASK_STK_SZ];          /* Task StopOnCollide stack                 */
OS_STK CtrlmotorStk[TASK_STK_SZ];           /* Task CtrlMotors stack                    */
OS_STK NavigStk[TASK_STK_SZ];               /* Task NavigRobot stack                    */

/* ------ Global shared variable -------*/
/* Ideally, this should be protected by a semaphore etc */
struct robostate
{
    int rspeed;                             /* right motor speed  (-100 -- +100)        */
    int lspeed;                             /* left motor speed  (-100 -- +100)         */
    char obstacle;                          /* obstacle? 1 = yes, 0 = no                */
} myrobot;

/*------High pririority task----------*/
void CheckCollision (void *data)
{
    for(;;)
    {
        if ( (robo_proxSensor() == 1) )             /* obstacle?                         */
            myrobot.obstacle = 1;                   /* signal obstacle present           */
        else
            myrobot.obstacle = 0;                   /* signal no obstacle                */

        OSTimeDlyHMSM(0, 0, 0, 100);                /* Task period ~ 100 ms              */
    }
}

/* Control robot Motors TASK */
void CntrlMotors (void *data)
{
    int speed_r, speed_l;

    for(;;)
    {
        speed_r = myrobot.rspeed;
        speed_l = myrobot.lspeed;
        robo_motorSpeed(speed_l, speed_r);
        OSTimeDlyHMSM(0, 0, 0, 5);                /* Task period ~ 5 ms              */
    }
}

/* --- Task for navigating robot ----
 * Write you own navigation task here
 */

void Navig (void *data)
{
    for (;;)
    {
        if (myrobot.obstacle == 0)              
        {
            if (robo_lightSensor() > 90) /* when light shines, we move forward */
            {              
                myrobot.rspeed   = 80;          /* move forward                   */
                myrobot.lspeed   = 80;
                robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
                OSTimeDlyHMSM(0, 0, 0, 5000);

                /*for (;;)
                {
                    // turn right slow
                    if (robo_lineSensor() == 1){
                        myrobot.rspeed   = 20;
                        myrobot.lspeed   = 40;

                    // turn right fast
                    } else if (robo_lineSensor() == 3){
                        myrobot.rspeed   = -30;
                        myrobot.lspeed   = 40;

                    // Forward slow
                    } else if (robo_lineSensor() == 2){
                        myrobot.rspeed   = 50;       
                        myrobot.lspeed   = 50;

                    // turn left fast
                    } else if (robo_lineSensor() == 6){
                        myrobot.rspeed   = 40;
                        myrobot.lspeed   = -30;
                        robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
                        OSTimeDlyHMSM(0, 0, 0, 400);

                    // turn left slow
                    } else if (robo_lineSensor() == 4){
                        myrobot.rspeed   = 50;
                        myrobot.lspeed   = -30;

                    } else if (robo_lineSensor() == 5){
                        myrobot.rspeed   = 40;
                        myrobot.lspeed   = 40;

                    } else if (robo_lineSensor() == 7){
                        myrobot.rspeed   = 50;       
                        myrobot.lspeed   = 50;
                    
                    // STOP
                    } else if (robo_lineSensor() == 0) {
                        myrobot.rspeed   = -35;
                        myrobot.lspeed   = -25;
                    } else if (myrobot.obstacle == 1)  {
                        myrobot.rspeed   =  0;  //stop
            			myrobot.lspeed   =  0;

            			robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            			OSTimeDlyHMSM(0, 0, 0, 600); 

            			myrobot.rspeed   =  -50;  //turn to right
            			myrobot.lspeed   =  80;

            			robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            			OSTimeDlyHMSM(0, 0, 0, 180); 

            			myrobot.rspeed   =  50;  //straight
            			myrobot.lspeed   =  50;

            			robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            			OSTimeDlyHMSM(0, 0, 2, 0);
                    }

                    robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
                    OSTimeDlyHMSM(0, 0, 0, 5);
                }*/
            }
            // turn right slow
            if (robo_lineSensor() == 1){
                myrobot.rspeed   = 50;
                myrobot.lspeed   = 80;

            // turn right fast
            } else if (robo_lineSensor() == 3){
                myrobot.rspeed   = -50;
                myrobot.lspeed   = 80;

            // Forward fast
            } else if (robo_lineSensor() == 2){
                myrobot.rspeed   = 80;       
                myrobot.lspeed   = 80;

            // turn left fast
            } else if (robo_lineSensor() == 6){
                myrobot.rspeed   = 80;
                myrobot.lspeed   = -60;

            // turn left slow
            } else if (robo_lineSensor() == 4){
                myrobot.rspeed   = 80;
                myrobot.lspeed   = -20;

            } else if (robo_lineSensor() == 5){
                myrobot.rspeed   = 80;
                myrobot.lspeed   = 80;

            } else if (robo_lineSensor() == 7){
                myrobot.rspeed   = 80;       
                myrobot.lspeed   = 80;
                
            // STOP
            } else if (robo_lineSensor() == 0) {
                myrobot.rspeed   = -50;
                myrobot.lspeed   = -35;
				OSTimeDlyHMSM(0, 0, 0, 100);
            }
        } else if (myrobot.obstacle == 1 && robo_lineSensor() == 0)  {
            myrobot.rspeed   =  0;  //stop
            myrobot.lspeed   =  0;

            robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            OSTimeDlyHMSM(0, 0, 0, 600); 

            myrobot.rspeed   =  -50;  //turn to right
            myrobot.lspeed   =  80;

            robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            OSTimeDlyHMSM(0, 0, 0, 200); 

            myrobot.rspeed   =  50;  //straight
            myrobot.lspeed   =  50;

            robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            OSTimeDlyHMSM(0, 0, 2, 0); 
        }else if(myrobot.obstacle == 1 && (robo_lineSensor() == 1 
		|| robo_lineSensor() == 2 || robo_lineSensor() == 3 || robo_lineSensor() == 4 
		|| robo_lineSensor() == 5 || robo_lineSensor() == 6 || robo_lineSensor() == 7)){
		    myrobot.rspeed   =  0;  //stop
            myrobot.lspeed   =  0;
			robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
            OSTimeDlyHMSM(0, 0, 0, 150);
			// turn right slow
            if (robo_lineSensor() == 1){
                myrobot.rspeed   = 50;
                myrobot.lspeed   = 80;

            // turn right fast
            } else if (robo_lineSensor() == 3){
                myrobot.rspeed   = -50;
                myrobot.lspeed   = 80;

            // Forward fast
            } else if (robo_lineSensor() == 2){
                myrobot.rspeed   = 80;       
                myrobot.lspeed   = 80;

            // turn left fast
            } else if (robo_lineSensor() == 6){
                myrobot.rspeed   = 80;
                myrobot.lspeed   = -60;

            // turn left slow
            } else if (robo_lineSensor() == 4){
                myrobot.rspeed   = 80;
                myrobot.lspeed   = -20;

            } else if (robo_lineSensor() == 5){
                myrobot.rspeed   = 80;
                myrobot.lspeed   = 80;

            } else if (robo_lineSensor() == 7){
                myrobot.rspeed   = 80;       
                myrobot.lspeed   = 80;
                
            // STOP
            } else if (robo_lineSensor() == 0) {
                myrobot.rspeed   = -50;
                myrobot.lspeed   = -35;
				//OSTimeDlyHMSM(0, 0, 0, 200);
            }
		}

        robo_motorSpeed(myrobot.lspeed, myrobot.rspeed);
        OSTimeDlyHMSM(0, 0, 0, 5);
    }
}

/*------Highest priority task----------*/
/* Create all other tasks here           */
void TaskStart( void *data )
{
    OS_ticks_init();                                        /* enable RTOS timer tick        */

    OSTaskCreate(CheckCollision,                            /* Task function                 */
                (void *)0,                                  /* nothing passed to task        */
                (void *)&ChkCollideStk[TASK_STK_SZ - 1],    /* stack allocated to task       */
                TASK_CHKCOLLIDE_PRIO);                      /* priority of task              */

    OSTaskCreate(CntrlMotors,                               /* Task function                 */
                (void *)0,                                  /* nothing passed to task        */
                (void *)&CtrlmotorStk[TASK_STK_SZ - 1],     /* stack allocated to task       */
                TASK_CTRLMOTOR_PRIO);                       /* priority of task              */

    OSTaskCreate(Navig,                                     /* Task function                 */
                (void *)0,                                  /* nothing passed to task        */
                (void *)&NavigStk[TASK_STK_SZ - 1],         /* stack allocated to task       */
                TASK_NAVIG_PRIO);                           /* priority of task              */

    while(1)
    {
        OSTimeDlyHMSM(0, 0, 5, 0);                          /* Task period ~ 5 secs          */
        robo_LED_toggle();                                  /* Show that we are alive        */
    }
}

int main( void )
{
    robo_Setup();                                          /* initialize HAL for RoboKar     */
    OSInit();                                              /* initialize UCOS-II kernel      */

    robo_motorSpeed(STOP_SPEED, STOP_SPEED);               /* Stop the robot                 */
    myrobot.rspeed   = STOP_SPEED;                         /* Initialize myrobot states      */
    myrobot.lspeed   = STOP_SPEED;
    myrobot.obstacle = 0;                                  /*  No collision                 */

    OSTaskCreate(TaskStart,                                /* create TaskStart Task          */
                (void *)0,
                (void *)&TaskStartStk[TASK_STK_SZ - 1],
                TASK_START_PRIO);
    robo_Honk(); 
    robo_wait4goPress();                                   /* Wait for to GO                 */
    OSStart();                                             /* Start multitasking             */
    while (1);                                             /* die here                       */
}
