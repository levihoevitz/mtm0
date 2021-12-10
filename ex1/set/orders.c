//
// Created by 97250 on 02/12/2021.
//

#include "orders.h"
#include "list.h"
#include "product.h"
#include "stdlib.h"
#include "matamikya_print.h"

struct order_t {
	unsigned int orderId;
	Set products;
	double total_profit;
	OrderStatus statusOrder;
	double price;
};


void* copyOrder(void* element)
{
	if (element == NULL) {
		return NULL;
	}
	Order order = element;
	Order new_order = creatOrder(order->orderId,
								 order->total_profit,
								 order->statusOrder,
								 order->price);
	if (new_order == NULL) {
		return NULL;
	}
	SET_FOREACH(Product, i, order->products)
	{
		if (setIsIn(new_order->products, i) != SET_SUCCESS) {
			setDestroy(new_order->products);
			free(new_order);
			return NULL;
		}
	}
	return new_order;
}

void freeOrder(void* order)
{
	if (order == NULL) {
		return;
	}
	Order ord = order;
	setDestroy(ord->products);
	free(ord);
}

int compareOrder(void* first_order_id, void* second_order_id)
{
	if (first_order_id == NULL || second_order_id == NULL) {
		return 0;
	}
	return *(int*) first_order_id - *(int*) second_order_id;
}

Order creatOrder(unsigned int orderId,
				 double total_profit,
				 OrderStatus statusOrder,
				 double price)
{

	Order new_order = malloc(sizeof(new_order));
	if (new_order == NULL) {
		return NULL;
	}
	new_order->orderId = orderId;
	new_order->total_profit = total_profit;
	new_order->products = setCreate(copyProduct, freeProduct, compareProduct);
	if (new_order->products == NULL) {
		free(new_order);
		return NULL;
	}
	new_order->statusOrder = statusOrder;
	new_order->price = price;
	return new_order;
}

unsigned int getOrderID(Order order)
{
	if (order == NULL) {
		return 0;
	}
	return order->orderId;
}

double getOrderTotalProfit(Order order)
{
	if (order == NULL) {
		return 0;
	}
	return order->total_profit;
}

OrderStatus getOrderStatus(Order order)
{
	if (order == NULL) {
		return 0;
	}
	return order->statusOrder;
}

double getOrderPrice(Order order)
{
	if (order == NULL) {
		return 0;
	}
	return order->price;
}

Set getOrderProducts(Order order)
{
	if (order == NULL) {
		return NULL;
	}
	return order->products;
}

void setOrderID(Order order, unsigned int orderId)
{
	if (order == NULL) {
		return;
	}
	order->orderId = orderId;
}

void setTotalProfit(Order order, double profit)
{
	if (order == NULL) {
		return;
	}
	order->total_profit = profit;
}

void setOrderStatus(Order order, OrderStatus statusOrder)
{
	if (order == NULL) {
		return;
	}
	order->statusOrder = statusOrder;
}

void setOrderPrice(Order order, double price)
{
	if (order == NULL) {
		return;
	}
	order->price = price;
}


bool compareOrderID(Order ord,unsigned int id)
{
	if (ord == NULL) {
		return false;
	}
	if (ord->orderId == id) {
		return true;
	}
	return false;
}

void changeStatusOrderToSent(Order order)
{
	if (order == NULL) {
		return;
	}
	order->statusOrder = ORDER_SENT;
}

///maybe its need to be in the matamikea
void CalculatesAndSetTheProfits(Order order)
{

}

bool IsTheAmountExists(Order order)
{
	return false;
}

void cancelOrder(Order order)
{

}

Product getProductFromOrderWithID(Order order, unsigned int productId)
{
	for (Product prod = setGetFirst(order->products); prod != NULL; prod = setGetNext(order->products)) {
		if (compareProductID(prod, productId)) {
			return prod;
		}
	}
	return NULL;
}

void addOrderProduct(Order order, unsigned int productId)
{

}

void removeOrderProduct(Order order, unsigned int productId)
{
	for (Product product = setGetFirst(order->products); product != NULL; product = setGetNext(order->products)) {
		if (getProductID(product) ==productId) {
			freeProduct(product);
			return;
		}
	}
}
