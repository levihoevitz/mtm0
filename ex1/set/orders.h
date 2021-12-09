//
// Created by 97250 on 02/12/2021.
//

#ifndef MTM0_ORDERS_H
#define MTM0_ORDERS_H
#include "set.h"
typedef struct order_t* Order;
typedef enum {
	ORDER_SENT,
	ORDER_IS_NOT_SENT,
}OrderStatus;

void* copyOrder(void* order);

void freeOrder(void* order);

int compareOrder(void* first_order_id, void* second_order_id);

Order creatOrder(unsigned int orderId,
				 double total_profit,
				 OrderStatus statusOrder,
				 double price);

unsigned int getOrderID(Order ord);

double getOrderTotalProfit(Order order);

Set getOrderProducts(Order order);

void setOrderID(Order order, unsigned int orderId);

bool compareOrderID(Order ord, unsigned int id);

void changeAmountOfProductInOrder(Order order,unsigned int productId, double amount);

void changeStatusOrderToSent(Order order);

bool IsTheAmountExists(Order order);

void CalculatesAndSetTheProfits(Order order);

void addOrderProduct(Order order,unsigned int productId);

void removeOrderProduct(Order order,unsigned int productId);

#endif //MTM0_ORDERS_H
