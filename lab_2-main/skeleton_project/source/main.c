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
    int previousFloor = -1;


    //Hvis heisen starter mellom etasjer, så flyttes den ned til nærmeste etasje
    int floor = elevio_floorSensor();
    int flag = 0;
    while (1) {
        if(!elevio_obstruction()){//Sjekker om det er sperring i heisdøra
            floor = elevio_floorSensor();
            if (floor == -1 && flag == 0) {
                elevio_motorDirection(DIRN_DOWN);
                flag = 1;
            }
            else if(floor != -1){
                elevio_motorDirection(DIRN_STOP);
                break;
            }
        }
        nanosleep(&(struct timespec){0, 1*1000*1000}, NULL); // Pauser her i 1 millisekund
    }




    //Kode for å overvåke knappetrykk
    while(1){
        if(!elevio_obstruction()){//Sjekker etter sperringer i døra
            previousFloor = currentFloor;

            // Oppdaterer gjeldende etasje
            floor = elevio_floorSensor();
            if (floor != -1) {
                currentFloor = floor;
            }

            //Sjekker etter knappetrykk inni heis
            for (int f = 0; f < N_FLOORS; f++) {
                if (elevio_callButton(f, 2)) {
                    targetFloor = f;
                    if (f > currentFloor) {
                        elevio_motorDirection(DIRN_UP);
                    } else if (f < currentFloor) {
                        elevio_motorDirection(DIRN_DOWN);
                    } else {
                        elevio_motorDirection(DIRN_STOP);
                        printf("Heisen er allerede i etasje: %d\n", f);
                    }
                }
            }

            // Stopper heisen når den har nådd destinasjonen sin
            if (currentFloor == targetFloor && currentFloor != -1) {
                elevio_motorDirection(DIRN_STOP);
                printf("Heisen har ankommet etasje: %d\n", currentFloor);
                targetFloor = -1; // Tilbakestiller destinasjonsetasje
            }

            //elevio_buttonLamp(1,2,1);
            //elevio_floorIndicator(1);


            
            //Lyser opp etasjelampen
            if(floor == 0){
                elevio_floorIndicator(0);
            }
            if(floor == 1){
                elevio_floorIndicator(1);
            }
            if(floor == 2){
                elevio_floorIndicator(2);
            }
            if(floor == N_FLOORS-1){
                elevio_floorIndicator(3);
            }
            
                    

            //Lyser opp knappen du trykker på
            for(int f = 0; f < N_FLOORS; f++){
                for(int b = 0; b < N_BUTTONS; b++){
                    int btnPressed = elevio_callButton(f, b);
                    if(btnPressed){
                        lampStatus[f][b] = 1; //Forteller at knapp er trykket
                        elevio_buttonLamp(f, b, 1);
                    }
                    else if(lampStatus[f][b]){
                        lampStatus[f][b] = 0; //Forteller at knapp er sluppet
                        elevio_buttonLamp(f, b, 0);
                    }
                }
            }
            
            //Lyser opp obstruction-lampa
            if(elevio_obstruction()){
                elevio_doorOpenLamp(1);
            } else {
                elevio_doorOpenLamp(0);
            }
            
            //Stopper programmet hvis stopp-knapp blir trykket
            if(elevio_stopButton()){
                elevio_motorDirection(DIRN_STOP);
                break;
            }
        }
        nanosleep(&(struct timespec){0, 1*1000*1000}, NULL); //Bestemmer hvor ofte knappene sjekkes
    }//Slutten på while-løkka
    

    

    return 0;
}//Slutten på main-funksjonen