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
void initialize(void)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        CLIENTS[i].client_id = 0;
    }
    for (i = 0; i < 500; i++)
    {
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
unsigned int addClient(unsigned char *name, float balance, unsigned int shares)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        if (CLIENTS[i].client_id == 0)
        {
            CLIENTS[i].name = name;
            CLIENTS[i].client_id = i + 1;
            CLIENTS[i].balance = balance;
            CLIENTS[i].shares = shares;
            printf("client added\n");
            return i + 1;
        }
    }
    return 0;
}

void removeClient(unsigned int client_id)
{

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
void printClients(void)
{
    int i;
    for (i = 0; i < 100; i++)
    {
        if (CLIENTS[i].client_id != 0)
        {
            printf("Client: %s\n    ID: %03d\n", CLIENTS[i].name, CLIENTS[i].client_id);
            printf("    Balance: $%.2f\n    shares: %d\n", CLIENTS[i].balance, CLIENTS[i].shares);
        }
    }
    return;
}
/*
placeOrder(num, price, client_id, type)
This function takes a number of shares, the id of the client who is placing the
order, and the type of order (0 - buy, 1 - sell). It will call 'processOrders()'
to process any new matching orders, after adding the new order.
Pre Conditions:
    initialize must have been run at least once.
Post Conditions:
    the new order was placed, and porcessed if a matching order existed.
Parameters:
    num: the number of shares to be sold/bought
    price: the price per share
    client_id: the id of an order to be checked against the rest
    type: 0 or 1 for buy or sell
Returns:
    0 or 1: regular and failed exit case
*/
int placeOrder(unsigned int num, float price, unsigned int client_id, unsigned int type)
{
    //Making sure that the clients balance is > than the price of the stock he wants to purcase
    //If not, we do a succesful exist.
    float total_stock_amount = (float)num * price;
    int i;
    printf("[DEBUG] Total Price: %.2f\n", total_stock_amount);
    if (type == 0 && total_stock_amount <= CLIENTS[client_id - 1].balance) {
        for (i = 0; i < 500; i++) {
            if (ORDERS[i].order_id == 0) {
                ORDERS[i].client_id = client_id;
                ORDERS[i].number = num;
                ORDERS[i].price = price;
                ORDERS[i].type = 0;
                ORDERS[i].order_id = i + 1;
                return processOrders(i + 1);
            }
        }
    }
    else if (type == 1 && num <= CLIENTS[client_id - 1].shares) {
        for (i = 0; i < 500; i++) {
            if (ORDERS[i].order_id == 0) {
                ORDERS[i].client_id = client_id;
                ORDERS[i].number = num;
                ORDERS[i].price = price;
                ORDERS[i].type = 1;
                ORDERS[i].order_id = i + 1;
                return processOrders(i + 1);
            }
        }
    }
    else {
        printf("[DEBUG] failed to place order\n");
        return 1;
    }
}

/*
processOrders(order_id)
This function takes the id of a new order and checks if it matches any existing
orders, using 'compareOrders()' to do this comparison. It then updates the each
client's shares and balance.
Pre Conditions:
    initialize must have been run at least once.
Parameters:
    order_id: the id of an order to be checked against the rest
Returns:
    0 or 1: regular and failed exit case
*/
int processOrders(unsigned int order_id) {
    int i;
    for (i = 0; i < 500; i++) {
        if (compareOrders(ORDERS[i], ORDERS[order_id - 1]) == 0){
            unsigned int client_A_id = ORDERS[order_id - 1].client_id;
            unsigned int client_B_id = ORDERS[i].client_id;
            float bill = (0 + ORDERS[order_id - 1].type)*(ORDERS[order_id - 1].price * (float)ORDERS[order_id - 1].number) + (0 + ORDERS[i].type)*(ORDERS[i].price * (float)ORDERS[i].number);
            printf("[DEBUG] Bill: %.2f\n", bill);
            if (ORDERS[client_A_id - 1].type == 1){
                CLIENTS[client_A_id - 1].balance += bill;
                CLIENTS[client_B_id - 1].balance -= bill;
                CLIENTS[client_A_id - 1].shares -= ORDERS[order_id - 1].number;
                CLIENTS[client_B_id - 1].shares += ORDERS[order_id - 1].number;
                ORDERS[order_id - 1].order_id = 0;
                ORDERS[i].order_id = 0;
            }
            else if (ORDERS[client_B_id - 1].type == 1){
                CLIENTS[client_B_id - 1].balance += bill;
                CLIENTS[client_A_id - 1].balance -= bill;
                CLIENTS[client_B_id - 1].shares -= ORDERS[order_id - 1].number;
                CLIENTS[client_A_id - 1].shares += ORDERS[order_id - 1].number;
                ORDERS[order_id - 1].order_id = 0;
                ORDERS[i].order_id = 0;
            }
            return 0;
        } 
    }
    return 1;
}

void printOrders(void)
{

    return;
}

/*
compareOrders(order A, order B)
Compares the two passed orders, if they are compatable it returns 0, else it
will return 1.
Parameters:
    A: one order
    B: another order
Returns:
    0 or 1: for match or non match respectively
*/
int compareOrders(order A, order B)
{
    if ((unsigned int)A.number == (unsigned int)B.number)
    {
        if (A.type == 0 && B.type == 1 && A.price >= B.price)
            return 0;
        if (A.type == 1 && B.type == 0 && A.price <= B.price)
            return 0;
    }
    return 1;
}