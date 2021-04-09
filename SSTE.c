#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SSTE.h"

/*
initialize(void)
Sets the id of each client and order in the array to 0, we'll use an id of 0
to mark empty or removed clients.
Pre Condition:
    nothing
Post Condition:
    Global arrays are in  itialized, and other functions can be used.
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
            strcpy(CLIENTS[i].name, name);
            //CLIENTS[i].name = name;
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
    if (CLIENTS[client_id - 1].client_id == client_id)
    {
        CLIENTS[client_id - 1].client_id = 0;

        strcpy(CLIENTS[client_id - 1].name, (unsigned char *)" ");
        //CLIENTS[client_id - 1].name = (unsigned char *)" ";
        CLIENTS[client_id - 1].balance = 0;
        CLIENTS[client_id - 1].client_id = 0;
        CLIENTS[client_id - 1].shares = 0;
        printf("client removed\n");

        for (int i = 0; i < 500; i++)
        {
            if (ORDERS[i].client_id == client_id)
                ORDERS[i].order_id = 0;
        }
        return;
    }
    printf("Client not found!\n");
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
    if (type == 0 && total_stock_amount <= CLIENTS[client_id - 1].balance)
    {
        for (i = 0; i < 500; i++)
        {
            if (ORDERS[i].order_id == 0)
            {
                ORDERS[i].client_id = client_id;
                ORDERS[i].number = num;
                ORDERS[i].price = price;
                ORDERS[i].type = 0;
                ORDERS[i].order_id = i + 1;
                return processOrders(i + 1);
            }
        }
    }
    else if (type == 1 && num <= CLIENTS[client_id - 1].shares)
    {
        for (i = 0; i < 500; i++)
        {
            if (ORDERS[i].order_id == 0)
            {
                ORDERS[i].client_id = client_id;
                ORDERS[i].number = num;
                ORDERS[i].price = price;
                ORDERS[i].type = 1;
                ORDERS[i].order_id = i + 1;
                return processOrders(i + 1);
            }
        }
    }
    else
    {
        printf("[DEBUG] failed to place order\n");
        return 1;
    }
    return 1;
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
int processOrders(unsigned int order_id)
{
    int i;
    for (i = 0; i < 500; i++)
    {
        if (compareOrders(ORDERS[i], ORDERS[order_id - 1]) == 0)
        {
            unsigned int client_A_id = ORDERS[order_id - 1].client_id;
            unsigned int client_B_id = ORDERS[i].client_id;
            float bill = (0 + ORDERS[order_id - 1].type) * (ORDERS[order_id - 1].price * (float)ORDERS[order_id - 1].number) + (0 + ORDERS[i].type) * (ORDERS[i].price * (float)ORDERS[i].number);
            if (ORDERS[client_A_id - 1].type == 1)
            {
                CLIENTS[client_A_id - 1].balance += bill;
                CLIENTS[client_B_id - 1].balance -= bill;
                CLIENTS[client_A_id - 1].shares -= ORDERS[order_id - 1].number;
                CLIENTS[client_B_id - 1].shares += ORDERS[order_id - 1].number;
                ORDERS[order_id - 1].order_id = 0;
                ORDERS[i].order_id = 0;
            }
            else if (ORDERS[client_B_id - 1].type == 1)
            {
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

/*
printOrders(void)
This function takes no paramaters and returns nothing and iterates through the 
global array of orders and prints each order with an id not equal to 0.
Pre Condition:
    initialize() must have been called once before hand.
Parameters:
    none
Returns:
    none 
*/
void printOrders(void)
{
    int i;
    printf("Current Orders:\n");
    for (i = 0; i < 100; i++)
    {
        if (ORDERS[i].order_id != 0)
        {
            printf("\nOrder ID: %03d \nClient Name: %s\n", ORDERS[i].order_id, CLIENTS[ORDERS[i].client_id - 1].name);
            if (ORDERS[i].type)
                printf("Buying: ");
            else
                printf(" Selling: ");
            printf("%d at $%.2f\n", ORDERS[i].number, ORDERS[i].price);
        }
    }
    return;
}

void UI()
{
    printf(" Welcome to the SSTE! \n");
    printf(" \n");

    while (1)
    {
        int userSelection;
        unsigned char name[30];
        int shares;
        float balance;
        unsigned int client_id;
        float price;
        unsigned int type;
        printf("1) Add a client\n2) Remove a client\n3) Print clients\n");
        printf("4) Place an order\n5) Print orders\n6) quit\n");
        printf("Enter a number (1 - 6): ");
        userSelection = getInputNum(1, 6);

        switch (userSelection)
        {
        case 1: // done
            // add client
            printf("Enter client name (max 30): ");
            scanf("%s", name);
            printf("Enter the amount of shares: ");
            shares = getInputNum(0, 10000);
            printf("Enter the account balance: ");
            balance = getInputFloat(0.0, 1000000.00);
            addClient(name, balance, shares);

            break;
        case 2:
            // remove client
            printf("Enter the Client you would like to remove ");
            scanf("%u", &client_id);
            removeClient(client_id);
            break;
        case 3: // done
            // print clients
            printClients();
            break;
        case 4:
            // place order
            printf("Enter how many shares you would like to buy or sell \n");
            shares = getInputNum(0, 10000);
            printf("Enter the price of the order \n");
            price = getInputFloat(0.0, 10000.00);
            printf("Enter your client ID \n");
            client_id = getInputNum(0, 100);
            printf("Enter the type of order, Enter 0 for buy, Enter 1 for Sell \n");
            type = getInputNum(0, 1);
            placeOrder(shares, price, client_id, type);
            break;
        case 5: // done
            // print orders
            printOrders();
            break;
        default: // done
            // quit
            exit(0);
        }
    }
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

/*
getInputNum(lower, upper)
This function takes a 'lower' bound and an 'upper' bound, inclusive, and will
ask for input until the given input meets the bound requirements and is a num.
Paramaters:
    lower: inputs less than this will be ignored
    upper: inputs above this will be ignored
Returns:
    num: the first input that fits within the bounds
*/
int getInputNum(int lower, int upper)
{
    int num = upper + 1;
    char term = '0';
    while (num > upper || num < lower)
    {
        if (scanf("%d", &num) == 0)
        {
            do
            {
                term = getchar();
            } while (!isdigit(term));
            ungetc(term, stdin);
        }
    }
    return num;
}

/*
getInputFloat(lower, upper)
This function takes a 'lower' bound and an 'upper' bound, inclusive, and will
ask for input until the given input meets the bound requirements and is a num.
Paramaters:
    lower: inputs less than this will be ignored
    upper: inputs above this will be ignored
Returns:
    num: the first input that fits within the bounds
*/
float getInputFloat(float lower, float upper)
{
    float num = upper + 1;
    char term = '0';
    while (num > upper || num < lower)
    {
        if (scanf("%f", &num) == 0)
        {
            do
            {
                term = getchar();
            } while (!isdigit(term));
            ungetc(term, stdin);
        }
    }
    return num;
}

int main()
{
    initialize();
    UI();
}
