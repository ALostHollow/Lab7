#include <stdio.h>
#include "SSTE.h"

/*
initialize(void)
Sets the id of each client and order in the array to 0, we'll use an id of 0
to mark empty or removed clients.
Pre Condition:
    nothing
Post Condition:
    Global arrays are initialized, and other functions can be used.
Parameters:
    none
Returns:
    none
*/
void initialize(void){
    int i;
    for (i = 0; i < 100; i++){
        CLIENTS[i].client_id = 0;
    }
    for (i = 0; i < 500; i++){
        ORDERS[i].order_id = 0;
    }
}

/*
addClient(unsigned char * name, float balance, unsigned int shares)
Takes a name, balace, and a number of shares, and updates the first found
client of id 0 in the global array with those values.
Pre Condition:
    initialize() must have been called once before hand.
Parameters:
    name: the name of the client.
    balance: the balance of the client's account
    share: the number of share owned by the client
Returns:
    none
*/
void addClient(unsigned char * name, float balance, unsigned int shares){
    int i;
    for (i = 0; i < 100; i++){
        if (CLIENTS[i].client_id == 0){
            CLIENTS[i].name = name;
            CLIENTS[i].client_id = i + 1;
            CLIENTS[i].balance = balance;
            CLIENTS[i].shares = shares;
            printf("client added\n");
            return;
        }
    }
    return;
}

void removeClient(unsigned int client_id){

    return;
}

/*
printClients(void)
Iterates through the global array and prints each client with an id not equal to 0.
Pre Condition:
    initialize() must have been called once before hand.
Parameters:
    none
Returns:
    none
*/
void printClients(void) {
    int i;
    for (i = 0; i < 100; i++){
        if (CLIENTS[i].client_id != 0) {
            printf("Client: %s\n    ID: %03d\n", CLIENTS[i].name, CLIENTS[i].client_id);
            printf("    Balance: $%.2f\n    shares: %d\n", CLIENTS[i].balance, CLIENTS[i].shares);
        }
    }
    return;
}

void placeOrder(unsigned int num, float price, unsigned int client_id, unsigned int type){

    return;
}

void printOrders(void) {

    return;
}

/*
compareOrders(order A, order B)
Compares the two passed orders, if they are compatable it returns 0, else it
will return 1.
*/
int compareOrders(order A, order B) {
    if ((unsigned int)A.number == (unsigned int)B.number) {
        if (A.type == 0 && B.type == 1 && A.price >= B.price)
            return 0;
        if (A.type == 1 && B.type == 0 && A.price <= B.price)
            return 0;
    }
    return 1;
}

void completeOrder(order A, order B) {
    
    return;
}