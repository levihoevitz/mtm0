//
// Created by 97250 on 02/12/2021.
//

#ifndef MTM0_ORDERS_H
#define MTM0_ORDERS_H
#include "set.h"

typedef struct order_t* Order;

void* copyOrder(void* order);

void freeOrder(void* order);

int compareOrder(void* first_order_id, void* second_order_id);

Order creatOrder(unsigned int orderId,
				 double total_profit,
				 double price);

unsigned int getOrderID(Order ord);

double getOrderTotalProfit(Order order);

Set getOrderProducts(Order order);

void setOrderID(Order order, unsigned int orderId);

bool compareOrderID(Order ord, unsigned int id);

//void changeStatusOrderToSent(Order order);

double CalculatesAndSetTheProfits(Order order);

void removeOrderProduct(Order order,unsigned int productId);

//void changeAmountOfProductInOrder(Order order,unsigned int productId, double amount);

#endif //MTM0_ORDERS_H
