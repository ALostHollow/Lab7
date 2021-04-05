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

    if (CLIENTS[client_id].client_id == client_id)
    {
        CLIENTS[client_id].client_id = 0;
        CLIENTS[client_id].name = " ";
        CLIENTS[client_id].balance = 0;
        CLIENTS[client_id].client_id = 0;
        CLIENTS[client_id].shares = 0;
        printf("client removed\n");
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
    printf("[DEBUG] Total Price: %.2f\n", total_stock_amount);
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
            printf("[DEBUG] Bill: %.2f\n", bill);
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

void printOrders(void)
{
    ;
    for (int i = 0; i < 100; i++)
    {
        if (ORDERS[i].order_id != 0)
        {
            printf("Client: %s\n    ID: %03d\n", ORDERS->client->name, ORDERS[i].order_id);
            printf("    Price per share: $%.2f\n    shares: %d\n", ORDERS[i].price, ORDERS[i].number);
        }
    }
    return;
}

void addRemoveHelper(client_ dummyArray[100])
{
    int userArg;
    printf(" If you would like to add a client: Please Enter 1 \n");
    printf(" if you would like to remove a client: Please Enter 2 \n ");
    scanf("%d", &userArg);

    if (userArg == 1)
    {
        unsigned char *name;
        float balance = 0;
        unsigned int shares = 0;
        printf("Enter the clients name? \n");
        scanf("%hhu", &name);
        addClient(name, balance, shares);
    }
    else if (userArg == 2)
    {
    }
    else
    {
        unsigned int client_id;
        printf("Enter the Client you would like to remove ");
        scanf("%u", client_id);
        removeClient(client_id);
    }
}

void placeOrderHelper()
{
    unsigned int num;
    float price;
    unsigned int client_id;
    unsigned int type;

    printf("Enter how many shares you would like to buy or sell \n");
    scanf("%u", &num);
    printf("Enter the price of the order \n");
    scanf("%f", &price);
    printf("Enter your client ID \n");
    scanf("%u", client_id);
    printf("Enter the type of order, Enter 1 for buy, Enter 2 for Sell \n");
    scanf("%u", type);
    placeOrder(num, price, client_id, type);
}

void viewOrderHelper()
{
}

void ClientGUI(client_ dummyArramy[100])
{
    int userArg;

    printf("If you would like to view all clients: Enter 1 \n");
    printf("If you would like to view details about a specific account: Enter 2 \n");
    scanf("%d", userArg);

    if (userArg == 1)
    {
        printClients();
    }
    else if (userArg == 2)
    {
        int loginDetails; // Variable to get Clients login ID
        int currentUser;  // Using the index of the char array like the pointer to the user. Maybe there's a better way to do this IDK
        Printf(" Please enter your Client ID \n");
        scanf("%d", &loginDetails);

        for (int i = 0; i < 100 < i++)
        {
            if (dummyArray[0].client_id == loginDetails)
            {
                currentUser = i;
                break;
            }
        }
        printf("Name: %s \n", dummyArramy[currentUser].name);
        printf("Client id: %u", dummyArramy[currentUser].client_id);
        printf(" Balance: %f", dummyArramy[currentUser].balance);
        printf("Shares: %u ", dummyArramy[currentUser].shares);
    }
}

void UI(client_ dummyArray[100])
{
    //client_ DummyArray[100]; // Placeholder array for the array of clients
    printf(" Welcome to the SSTE! \n");
    printf(" \n");

    int userSelection;

    printf(" Please Enter a number from the following options \n");

    printf("If you would like to add or remove a client:  Enter 1 \n");
    printf("If you Would like to view your account balance and details: Enter 2 \n");
    printf("If you would like to place an order to buy or sell securities: Enter 3 \n");
    printf("If you would like to check the status of orders on the exchange: Enter 4 \n");
    scanf(" %d", &userSelection);

    if (userSelection == 1)
    {
        addRemoveHelper();
    }

    else if (userSelection == 2)
    {
    }
    else if (userSelection == 3)
    {
        placeOrderHelper();
    }
    else if (userSelection == 4)
    {
        printOrders
    }
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