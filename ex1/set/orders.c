#include "orders.h"
#include "product.h"
#include "stdlib.h"

struct order_t {
	unsigned int orderId;
	Set products;
};


void* copyOrder(void* element)
{
	if (element == NULL) {
		return NULL;
	}
	Order order = element;
	Order new_order = creatOrder(order->orderId);
	if (new_order == NULL) {
		return NULL;
	}
	SET_FOREACH(Product, i, order->products) {
		if (setIsIn(new_order->products, i) != SET_SUCCESS) {
			setDestroy(new_order->products);
			free(new_order);
			return NULL;
		}
	}
	return new_order;
}

void freeOrder(void* element)
{
	if (element == NULL) {
		return;
	}
	Order order = element;
	setDestroy(order->products);
	free(order);
}

int compareOrder(void* first_element, void* second_element)
{
	if (first_element == NULL || second_element == NULL) {
		return 0;
	}
	Order first_order = first_element;
	Order second_order = second_element;

	return first_order->orderId - second_order->orderId;
}

Order creatOrder(unsigned int orderId)
{

	Order new_order = malloc(sizeof(*new_order));
	if (new_order == NULL) {
		return NULL;
	}
	new_order->orderId = orderId;
	new_order->products = setCreate(copyProduct, freeProduct, compareProduct);
	if (new_order->products == NULL) {
		free(new_order);
		return NULL;
	}
	return new_order;
}

unsigned int getOrderID(Order order)
{
	if (order == NULL) {
		return 0;
	}
	return order->orderId;
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

double getOrderProfits(Order order)
{
	if (order == NULL) {
		return 0;
	}
	double total_profit_of_order = 0;
	for (Product product = setGetFirst(order->products); product != NULL; product = setGetNext(order->products)) {
		total_profit_of_order += getProductTotalPrice(product);
	}
	return total_profit_of_order;
}


void removeOrderProductByID(Order order, unsigned int productId)
{
	for (Product product = setGetFirst(order->products); product != NULL; product = setGetNext(order->products)) {
		if (getProductID(product) == productId) {
			setRemove(order->products, product);
			return;
		}
	}
}
