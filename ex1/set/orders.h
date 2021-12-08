//
// Created by 97250 on 02/12/2021.
//

#ifndef MTM0_ORDERS_H
#define MTM0_ORDERS_H
#include "amount_set.h"
typedef struct order_t* Order;
typedef enum statusOrder{
	ORDER_SENT,
	ORDER_IS_NOT_SENT,
}StatusOrder;
Order creatOrder(unsigned int orderId,
				 double total_profit,
				 StatusOrder statusOrder,
				 double price);
#endif //MTM0_ORDERS_H
