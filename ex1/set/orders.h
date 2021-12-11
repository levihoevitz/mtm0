#ifndef MTM0_ORDERS_H
#define MTM0_ORDERS_H
#include "set.h"

typedef struct order_t* Order;

void* copyOrder(void* order);

void freeOrder(void* element);

int compareOrder(void* first_element, void* second_element);

Order creatOrder(unsigned int orderId);

unsigned int getOrderID(Order ord);

Set getOrderProducts(Order order);

void setOrderID(Order order, unsigned int orderId);

double getOrderProfits(Order order);

void removeOrderProductByID(Order order, unsigned int productId);

#endif //MTM0_ORDERS_H
