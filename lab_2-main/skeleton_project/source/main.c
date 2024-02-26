#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"


int main(){
    elevio_init();

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_STOP);


    while(1){
        //elevio_buttonLamp(1,2,1);
        //elevio_floorIndicator(1);
        int floor = elevio_floorSensor();


        /*
        //Lyser opp etasjelampen
        if(floor == 0){
            //elevio_motorDirection(DIRN_UP);
            //printf("Etasje 1\n");
            elevio_floorIndicator(0);
        }
        if(floor == 1){
            //printf("Etasje 2\n");
            elevio_floorIndicator(1);
        }
        if(floor == 2){
            //printf("Etasje 3\n");
            elevio_floorIndicator(2);
        }
        if(floor == N_FLOORS-1){
            //elevio_motorDirection(DIRN_DOWN);
            //printf("Etasje 4\n");
            elevio_floorIndicator(3);
        }
        */
        


        //Trykker på knapp og heisen flytter seg dit
        printf("%d\n", floor);
        int OnsketEtasje = 4;
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);


                if(b == 2 && f == 0 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("1\n");
                }
                if(b == 2 && f == 1 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("1\n");
                }
                if(b == 2 && f == 2 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("1\n");
                }
                if(b == 2 && f == 3 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("1\n");
                }
                if(floor == f){
                        elevio_motorDirection(DIRN_STOP);
                    }


                }
        }

        /*
        int OnsketEtasje = 4;
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);

                //Sjekker hvilken knapptype
                if(b == 2 && f == 0 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("1\n");
                }
                else if(b == 2 && f == 1 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("2\n");
                }int OnsketEtasje = 4;
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);

                //Sjekker hvilken knapptype
                if(b == 2 && f == 0 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("1\n");
                }
                else if(b == 2 && f == 2 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                  else if(b == 2 && f == 3 && btnPressed == 1){
                    OnsketEtasje = f;
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    //printf("4\n");
                }
            }
        }
        */

        

        /*
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
                //printf("%d%d%d\n", btnPressed, f, b);
            }
        }
        */

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }
    

    

    return 0;
    }
