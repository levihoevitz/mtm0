#ifndef MTM0_ORDERS_H
#define MTM0_ORDERS_H
#include "set.h"

typedef struct order_t* Order;

/**
 * copyProduct: Copies an existing order on a product.
 *
 * @param order - Generic pointer to a product.
 * @return
 *     NULL -  if element/newOrder are Null, or fails to place the order.
 *     new_order - if copies the order successfully.
 */
void* copyOrder(void* order);

/**
 * freeOrder: cancels an order on a product.
 *
 * @param element - Generic pointer to a product.
 * @return
 *     NULL -  if element is Null.
 *     void - if cancels order successfully.
 */
void freeOrder(void* element);

/**
 * compareOrder: compares two orders.
 *
 * @param first_element - pointer to the first order.
 * @param second_element - pointer to the second order.

 * @return
 *     0 -  if one of the orders doesn't exist.
 *     >0 - if the first order was places before the seconds order.
 *     <0 - if the first order was places after the seconds order.
 */
int compareOrder(void* first_element, void* second_element);

/**
 * creatProduct: Places an order on a product.
 *
 * @param orderId - an id of a product.
 * @return
 *     NULL -  if newOrder/newOrder.products are Null.
 *     new_order - if places the order successfully.
 */
 Order creatOrder(unsigned int orderId);

/**
 * getOrderID: Gets the ID of a given order.
 *
 * @param product - a given order.

 * @return
 *     Null - if product is Null.
 *     order->orderId - if order exists.
 */
unsigned int getOrderID(Order ord);

/**
 * getOrderProducts: Gets the products of a given order.
 *
 * @param product - a given order.

 * @return
 *     Null - if product is Null.
 *     order->products - if order exists.
 */
Set getOrderProducts(Order order);

/**
 * setOrderID: Sets the ID of a given order.
 *
 * @param product - a given order.

 * @return
 *     Null - if product is Null.
 *     void - if the ID is set successfully.
 */
void setOrderID(Order order, unsigned int orderId);

/**
 * getOrderProfits: Gets how much profit will be gained from the order.
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     total_profit_of_order - if order exists.
 */
double getOrderProfits(Order order);

/**
 * removeOrderProductByID: Cancels an order using id.
 *
 * @param order - a given product.
 * @param productId - a given id

 * @return
 *     void - if order is removed successfully.
 */
void removeOrderProductByID(Order order, unsigned int productId);

bool addOrderProductByID(Order order, unsigned int productId);

#endif //MTM0_ORDERS_H
