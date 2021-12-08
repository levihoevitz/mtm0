//
// Created by 97250 on 02/12/2021.
//

#include "orders.h"
#include "list.h"
#include "product.h"
#include "stdlib.h"

struct order_t {
	unsigned int orderId;
	AmountSet products;
	double total_profit;
	StatusOrder statusOrder;
	double price;
};

void* copyProduct(void* order)
{
	if (order == NULL) {
		return NULL;
	}
	Order ord = order;
	Order new_order = creatOrder(ord->orderId,
								 ord->total_profit,
								 ord->statusOrder,
								 ord->price);
	if (new_order == NULL) {
		return NULL;
	}
	AS_FOREACH(Product,i,ord->products){
		if (asRegister(new_order->products,i)!=AS_SUCCESS){
			asDestroy(new_order->products);
			free(new_order);
			return NULL;
		}
	}
	return new_order;
}

void freeProduct(void* order)
{
	if (order == NULL) {
		return;
	}
	Order ord = order;
	asDestroy(ord->products);
	free(ord);
}

int compareProduct(void* first_order_id, void* second_order_id)
{
	if (first_order_id == NULL || second_order_id == NULL) {
		return 0;
	}
	return *(int*) first_order_id - *(int*) second_order_id;
}

Order creatOrder(unsigned int orderId,
				 double total_profit,
				 StatusOrder statusOrder,
				 double price)
{

	Order new_order = malloc(sizeof(new_order));
	if (new_order == NULL) {
		return NULL;
	}
	new_order->orderId = orderId;
	new_order->total_profit = total_profit;
	new_order->products = asCreate(copyProduct, freeProduct, compareProduct);
	if (new_order->products == NULL) {
		free(new_order);
		return NULL;
	}
	new_order->statusOrder = statusOrder;
	new_order->price = price;
	return new_order;
}
