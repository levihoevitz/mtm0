//
// Created by 97250 on 02/12/2021.
//

#ifndef MTM0_ORDERS_H
#define MTM0_ORDERS_H
#include "amount_set.h"
typedef struct order_t* Order;
typedef enum {
	ORDER_SENT,
	ORDER_IS_NOT_SENT,
}OrderStatus;
Order creatOrder(unsigned int orderId,
				 double total_profit,
				 OrderStatus statusOrder,
				 double price);
unsigned int getOrderID(Order ord);
bool compareOrderID(Order ord, int id);
void changeAmountOfProductInOrder(Order order,unsigned int productId, double amount);
void changeStatusOrderToSent(Order order);
bool IsTheAmountExists(Order order);
void CalculatesTheProfits(Order order);
#endif //MTM0_ORDERS_H
