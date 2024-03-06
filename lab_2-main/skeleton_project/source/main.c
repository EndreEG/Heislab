#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include <stdbool.h>


//Definerer køsystem for kabinknapper
typedef struct node {
    int val;
    struct node * next;
} kabinKnapper;


int main(){
    elevio_init();

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_STOP);

    int currentFloor = -1; // Setter verdien til gjeldende etasje til en ugyldig verdi
    int targetFloor = -1; //Variabel for destinasjons-etasjen
    int previousFloor = -1;
    int nextFloor;
    bool alreeadyPrinted = false;
    int retning;
    int firstUpDown;

    kabinKnapper * head = NULL; //Køsystem for knappetrykk inni heis
    kabinKnapper * opp = NULL;
    kabinKnapper * ned = NULL;



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
            //Sjekker om heisen går opp eller ned
            if(previousFloor != currentFloor && currentFloor != nextFloor){
                if(previousFloor > currentFloor){
                    printf("Heisen går nedover\n");
                    retning = -1;
                }
                else if(previousFloor < currentFloor && previousFloor != -1 && currentFloor != nextFloor){
                    printf("Heisen går oppover\n");
                    retning = 1;
                }
            }
            previousFloor = currentFloor; //Oppdaterer forrige etasje
            //if(opp == NULL && ned == NULL){
            //    firstUpDown = 0;
            //}

            // Oppdaterer gjeldende etasje
            floor = elevio_floorSensor();
            if (floor != -1) {
                currentFloor = floor;
            }

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

            //Legger til etasjer i køsystemet ved knappetrykk i kabinen
            for (int f = 0; f < N_FLOORS; f++) {
                if (elevio_callButton(f, 2)) {
                    if(checkList(head, f) == 1){ //Sjekker om knappetrykket allerede er i lista. 0 = i liste, 1 = ikke i liste
                        pushEnd(&head, f);
                        elevio_buttonLamp(f, 2, 1);
                    }
                    printList(head);
                }
            }
            //Legger til trykk på opp-knapper
            for (int f = 0; f < N_FLOORS-1; f++) {
                if (elevio_callButton(f, 0)) {
                    if(checkList(opp, f) == 1){ //Sjekker om knappetrykket allerede er i lista. 0 = i liste, 1 = ikke i liste
                        pushEnd(&opp, f);
                        elevio_buttonLamp(f, 0, 1);
                        //if(firstUpDown == 0){
                        //    firstUpDown = 1;
                        //}
                    }
                    printList(opp);
                }
            }
            //Legger til trykk på ned-knapper
            for (int f = 1; f < N_FLOORS; f++) {
                if (elevio_callButton(f, 1)) {
                    if(checkList(ned, f) == 1){ //Sjekker om knappetrykket allerede er i lista. 0 = i liste, 1 = ikke i liste
                        pushEnd(&ned, f);
                        elevio_buttonLamp(f, 1, 1);
                        //if(firstUpDown == 0){
                        //    firstUpDown = -1;
                        //}
                    }
                    printList(ned);
                }
            }
            

            //Kjører heisen til neste etasje i køen
            nextFloor = getFirst(head);
            if(head == NULL){
                nextFloor = getFirst(opp);
                if(opp == NULL){
                    nextFloor = getFirst(ned);
                }
                /*
                printf("First: %d\n", firstUpDown);
                if(firstUpDown == 1){
                    nextFloor = getFirst(opp);
                }
                else if(firstUpDown == -1){
                    nextFloor = getFirst(ned);
                }
                */
                
            }
            if (nextFloor > currentFloor && nextFloor != -1) {
                elevio_motorDirection(DIRN_UP);
                alreeadyPrinted = false;
            }
            else if (nextFloor < currentFloor && nextFloor != -1) {
                elevio_motorDirection(DIRN_DOWN);
                alreeadyPrinted = false;
            }
            else {//Stopper når heisen har ankommet
                elevio_motorDirection(DIRN_STOP);
                elevio_buttonLamp(currentFloor, 2, 0);
                pop(&head);
                retning = 0;
                if(currentFloor != N_FLOORS-1){
                    elevio_buttonLamp(currentFloor, 0, 0);
                    removeValue(&opp, currentFloor);
                }
                if(currentFloor != 0){
                    elevio_buttonLamp(currentFloor, 1, 0);
                    removeValue(&ned, currentFloor);
                }
                //firstUpDown = 0;
                if(!alreeadyPrinted && nextFloor != -1){
                    printf("Heisen har ankommet %d. etasje\n", nextFloor);
                    alreeadyPrinted = true;
                    elevio_doorOpenLamp(1);
                    stopp(&head, &opp, &ned);
                    elevio_doorOpenLamp(0);
                }
            }
            //Plukker opp folk på veien
            if(checkList(head, currentFloor) == 0 || (checkList(opp, currentFloor) == 0 && retning == 1) || (checkList(ned, currentFloor) == 0 && retning == -1)){
                elevio_buttonLamp(currentFloor, 2, 0);
                if(currentFloor != N_FLOORS-1){//Skrur av opp_lys
                    elevio_buttonLamp(currentFloor, 0, 0);
                }
                if(currentFloor != 0){//Skrur av ned_lys
                    elevio_buttonLamp(currentFloor, 1, 0);
                }
                elevio_doorOpenLamp(1);
                stopp(&head, &opp, &ned);
                elevio_doorOpenLamp(0);
                removeValue(&head, currentFloor);
                if(currentFloor != N_FLOORS-1){//Fjerner fra opp-liste
                    removeValue(&opp, currentFloor);
                }
                if(currentFloor != 0){//Fjerner fra ned-liste
                    removeValue(&ned, currentFloor);
                }
            }

            // Stopper heisen når den har nådd destinasjonen sin
            /*
            if (currentFloor == targetFloor && currentFloor != -1 && previousFloor != currentFloor) {
                elevio_motorDirection(DIRN_STOP);
                printf("Heisen har ankommet etasje: %d\n", currentFloor);
                targetFloor = -1; // Tilbakestiller destinasjonsetasje
                pop(&head);//Fjerner etasje fra lista
                printList(head);
            }  
            */
                 
            
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
        nanosleep(&(struct timespec){0, 1*1000*1000}, NULL); //Bestemmer hvor ofte while-løkka repeteres
    }//Slutten på while-løkka
    
    // Deallokerer minnet for å unngå minnelekkasje
    kabinKnapper *current = head;
    kabinKnapper *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    kabinKnapper *current_opp = opp;
    kabinKnapper *next_opp;
    while (current_opp != NULL) {
        next_opp = current_opp->next;
        free(current_opp);
        current_opp = next_opp;
    }
    kabinKnapper *current_ned = ned;
    kabinKnapper *next_ned;
    while (current_ned != NULL) {
        next_ned = current_ned->next;
        free(current_ned);
        current_ned = next_ned;
    }
    return 0;
}//Slutten på main-funksjonen







//Legger til element i slutten av lista
void pushEnd(kabinKnapper **headRef, int val) {
    kabinKnapper *newNode = (kabinKnapper *)malloc(sizeof(kabinKnapper));
    if (newNode == NULL) {
        return 1; //Stopper minneallokeringen hvis den feiler (ikke nok minneplass)
    }
    newNode->val = val;
    newNode->next = NULL;
    //Hvis lista er tom, så blir denne noden til head
    if (*headRef == NULL) {
        *headRef = newNode;
        return;
    }
    //Hvis det allerede er elementer i lista, så går den til slutten og legger til der
    kabinKnapper *current = *headRef;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}


//Fjerner første element i lista
int pop(kabinKnapper ** head) {
    int retval = -1;
    kabinKnapper * next_node = NULL;
    if (*head == NULL) {
        return -1;
    }
    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}


//Printer kø-lista
void printList(kabinKnapper * head) {
    kabinKnapper * current = head;

    while (current != NULL) {
        printf("%d, ", current->val);
        current = current->next;
    }
    printf("\n");
}

//Sjekker om en verdi er i lista
int checkList(kabinKnapper * head, int f){
    kabinKnapper * current = head;
    while (current != NULL){
        if(current->val == f){
            return 0;//Hvis den finner verdien i lista, så returneres 0
        }
        current = current->next;
    }
    return 1; //Hvis den ikke finner verdien, så returneres 1
}



//Returnerer første element i lista
int getFirst(kabinKnapper * head) {
    if (head == NULL) {
        //printf("Listen er tom.\n");
        return -1; // Returverdien indikerer en feiltilstand
    }

    return head->val;
}

//Fjerner et spesifikt element i lista etter index
int remove_by_index(kabinKnapper ** head, int n) {
    int i = 0;
    int retval = -1;
    kabinKnapper * current = *head;
    kabinKnapper * temp_node = NULL;
    if (n == 0) {
        return pop(head);
    }
    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next;
    }
    if (current->next == NULL) {
        return -1;
    }
    temp_node = current->next;
    retval = temp_node->val;
    current->next = temp_node->next;
    free(temp_node);

    return retval;
}

// Funksjon for å fjerne en spesifikk verdi fra listen
void removeValue(kabinKnapper** headRef, int value) {
    kabinKnapper* current = *headRef;
    kabinKnapper* prev = NULL;
    // Søk gjennom listen for å finne verdien
    while (current != NULL && current->val != value) {
        prev = current;
        current = current->next;
    }
    // Hvis verdien ikke ble funnet, avslutt funksjonen
    if (current == NULL) {
        return;
    }
    // Endre pekerne for å hoppe over noden med verdien som skal fjernes
    if (prev == NULL) { // Hvis noden som skal fjernes er den første i listen
        *headRef = current->next;
    } else {
        prev->next = current->next;
    }
    free(current); // Frigjør minnet som ble allokert for noden
}



//Stopper heisen i tre sekunder, men kan fortsatt legge inn bestillinger
void stopp(kabinKnapper ** head, kabinKnapper ** opp,  kabinKnapper ** ned){
    elevio_motorDirection(DIRN_STOP);
    for (int i = 0; i < 175; i++){ //Pauser heisen i tre sekunder, men fortsatt mulig å legge inn bestillinger
        for (int f = 0; f < N_FLOORS; f++) {
            if (elevio_callButton(f, 2)) {//Sjekker etter trykk på etasjeknapp
                if(checkList(*head, f) == 1){ //Sjekker om knappetrykket allerede er i lista. 0 = i liste, 1 = ikke i liste
                    pushEnd(head, f);
                    elevio_buttonLamp(f, 2, 1);
                }
            printList(*head);
            }
            if (elevio_callButton(f, 0)) {//Sjekker etter trykk på opp-knapp
                if(checkList(*opp, f) == 1){ //Sjekker om knappetrykket allerede er i lista. 0 = i liste, 1 = ikke i liste
                    pushEnd(opp, f);
                    elevio_buttonLamp(f, 0, 1);
                }
            printList(*opp);
            }
            if (elevio_callButton(f, 1)) {//Sjekker etter trykk på ned-knapp
                if(checkList(*ned, f) == 1){ //Sjekker om knappetrykket allerede er i lista. 0 = i liste, 1 = ikke i liste
                    pushEnd(ned, f);
                    elevio_buttonLamp(f, 1, 1);
                }
            printList(*ned);
            }
        }
        nanosleep(&(struct timespec){0, 0.5*1000*1000}, NULL);
    }
}
