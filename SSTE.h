/*
The header file for lab 7
*/
// STRUCTS:
typedef struct order_
{
    unsigned int number;
    float price;
    unsigned int type; // 0 for buy, 1 for sell
    //struct client_ *client;
    unsigned int client_id;
    unsigned int order_id;
} order;

typedef struct client_
{
    unsigned char *name;
    unsigned int client_id;
    float balance;
    unsigned int shares;
} client;

// GLOBAL VARIABLES:
struct client_ CLIENTS[100];
struct order_ ORDERS[500];

// UI:
//  add client
//  place order
//  print client into

// SSTE.c module:
//  keep track of upto 500 outstanding orders
//  keep track of upto 100 clients
//  when a new order is added, check for matching opposite orders
//  validate new orders
//  validate when processing orders

// FUNCTIONS:

void initialize(void); // Done
// adds ids of 0 to each element in the global arrays

unsigned int addClient(unsigned char *name, float balance, unsigned int shares); // Done
// adds a new client, if there are less than 100 already present.

void removeClient(unsigned int client_id);
// takes a client id and removes it from the array of clients
// removes all orders from the array of outstanding orders for that client

void printClients(void); // Done
// prints the clients currently in the global array of clients

int placeOrder(unsigned int num, float price, unsigned int client_id, unsigned int type); // Done
// validates the order: checks client for enough money/shares for buy and sell respectively
// checks checks each

void printOrders(void);
// prints the outstanding orders in the global array of orders

int compareOrders(order A, order B); // Done
// a comparision function for orders, returns a 0 if compatable or a 1 if not compatable.

int processOrders(unsigned int order_id); // Done
                                          // takes an order'is id and compares it against all other orders, processing them if they match