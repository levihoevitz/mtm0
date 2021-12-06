//
// Created by 97250 on 02/12/2021.
//

#include "orders.h"
#include "list.h"
#include "amount_set.h"

struct order_t {
	unsigned int orderId;
	AmountSet products;
	double total_profit;
	StatusOrder statusOrder;
	double price;
};
