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

    int lampStatus[N_FLOORS][N_BUTTONS] = {0};//Variabel som forteller om knapp er trykket ned eller ikke

    int currentFloor = -1; // Setter verdien til gjeldende etasje til en ugyldig verdi
    int targetFloor = -1; //Variabel for destinasjons-etasjen

    while(1){
        // Oppdaterer gjeldende etasje
        int floor = elevio_floorSensor();
        if (floor != -1) {
            currentFloor = floor;
        }

        //Sjekker etter knappetrykk inni heis
        for (int f = 0; f < N_FLOORS; f++) {
            if (elevio_callButton(f, 2)) {
                targetFloor = f;
                if (currentFloor == -1) {
                    //Hvis heisen starter i en posisjon mellom etasjer, så flytter den seg til nærmeste etasje
                    if (f > 0) {
                        elevio_motorDirection(DIRN_DOWN);
                    } else {
                        elevio_motorDirection(DIRN_UP);
                    }
                } else if (f > currentFloor) {
                    elevio_motorDirection(DIRN_UP);
                } else if (f < currentFloor) {
                    elevio_motorDirection(DIRN_DOWN);
                } else {
                    elevio_motorDirection(DIRN_STOP);
                    printf("Heisen er allerede i etasjen %d\n", f);
                }
            }
        }

        // Stopper heisen når den har nådd destinasjonen sin
        if (currentFloor == targetFloor) {
            elevio_motorDirection(DIRN_STOP);
            printf("Heisen har nådd destinasjonen sin %d\n", currentFloor);
            targetFloor = -1; // Tilbakestiller destinasjonsetasje
        }

        //elevio_buttonLamp(1,2,1);
        //elevio_floorIndicator(1);


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
        

       /*
        //Trykker på knapp og heisen flytter seg dit
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);

                //Inni heis, trykk til 1
                if(b == 2 && f == 0 && btnPressed == 1){
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                }
                //Inni heis, trykk til 2
                if(b == 2 && f == 1 && btnPressed == 1){
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                }
                //Inni heis, trykk til 3
                if(b == 2 && f == 2 && btnPressed == 1){
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                }
                //Inni heis, trykk til 4
                if(b == 2 && f == 3 && btnPressed == 1){
                    //Sjekker om vi må opp eller ned
                    if(floor < f){
                        elevio_motorDirection(DIRN_UP);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                    else if(floor > f){
                        elevio_motorDirection(DIRN_DOWN);
                        printf("floor=%d, f=%d\n", floor, f);
                    }
                }
                //Stopper når den ankommer riktig etasje
                if(floor == f){
                    elevio_motorDirection(DIRN_STOP);
                }
            }
        }
        */
        

        

        //Lyser opp knappen du trykker på
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if(btnPressed){
                    lampStatus[f][b] = 1; //Forteller at knapp er trykket
                    elevio_buttonLamp(f, b, 1);
                }
                else if(lampStatus[f][b]){
                    lampStatus[f][b] = 0; //Forteller at knapp er trykket
                    elevio_buttonLamp(f, b, 0);
                }
            }
        }
        
        //Lyser opp obstruction-lampa
        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        //Stopper programmet hvis stopp-knapp blir trykket
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
        nanosleep(&(struct timespec){0, 1*1000*1000}, NULL);
    }//Slutten på while-løkka
    

    

    return 0;
}//Slutten på main-funksjonen