#include "matamikya.h"
#include <stdlib.h>
#include "set.h"
#include "product.h"
#include "string.h"
#include "orders.h"
#include "matamikya_print.h"
#include "math.h"

#define EPSILON 0.001

struct Matamikya_t {
	Set Products;
	Set orders;
};

Matamikya matamikyaCreate()
{
	Matamikya new_matamikya = malloc(sizeof(*new_matamikya));
	if (new_matamikya == NULL) {
		return NULL;
	}

	new_matamikya->Products = setCreate(copyProduct,
										freeProduct,
										compareProduct);
	if (new_matamikya->Products == NULL) {
		free(new_matamikya);
		return NULL;
	}

	new_matamikya->orders = setCreate(copyOrder,
									  freeOrder,
									  compareOrder);
	if (new_matamikya->Products == NULL) {
		setDestroy(new_matamikya->orders);
		free(new_matamikya);
		return NULL;
	}

	return new_matamikya;
}

void matamikyaDestroy(Matamikya matamikya)
{
	if (matamikya == NULL) {
		return;
	}
	setDestroy(matamikya->Products);
	setDestroy(matamikya->orders);
	free(matamikya);
}

static bool isTheNameLegal(const char* name)
{
	if (strlen(name) == 0) {
		return false;
	}
	int index = 0;
	while (name[index]) {
		if (!(('a' <= name[index] && name[index] <= 'z') || ('A' <= name[index] && name[index] <= 'Z') ||
			  ('0' <= name[index] && name[index] <= '9'))) {
			return false;
		}
		index++;
	}
	return true;
}

static bool isTheAmountTypeLegal(const double amount, const MatamikyaAmountType amountType)
{
	if (amountType == MATAMIKYA_INTEGER_AMOUNT) {
		if (fabs(amount - (int) amount) >= EPSILON) {
			return false;
		}
		return true;
	}

	if (amountType == MATAMIKYA_HALF_INTEGER_AMOUNT) {
		if (fabs(amount - ((int) amount + 0.5)) >= EPSILON) {
			return false;
		}
		return true;
	}
	return true;
}

static bool isProductIsIn(Set products, unsigned int id)
{
	SET_FOREACH(Product, product, products) {
		if (getProductID(product) == id)
			return true;
	}
	return false;
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char* name,
							  const double amount, const MatamikyaAmountType amountType,
							  const MtmProductData customData, MtmCopyData copyData,
							  MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
	if (matamikya == NULL || name == NULL || customData == NULL ||
		copyData == NULL || freeData == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	if (!isTheNameLegal(name)) {
		return MATAMIKYA_INVALID_NAME;
	}
	if (!isTheAmountTypeLegal(amount, amountType)) {
		return MATAMIKYA_INVALID_AMOUNT;
	}
	if (isProductIsIn(matamikya->Products, id)) {
		return MATAMIKYA_PRODUCT_ALREADY_EXIST;
	}
	Product new_product = creatProduct(name, id, amount, amountType,
									   customData, copyData, freeData, prodPrice);
	if (new_product == NULL) {
		return MATAMIKYA_OUT_OF_MEMORY;
	}

	/*if (setIsIn(matamikya->Products, new_product)) {
		freeProduct(new_product);
		return MATAMIKYA_PRODUCT_ALREADY_EXIST;
	}*/
	if (setAdd(matamikya->Products, new_product) != SET_SUCCESS) {
		freeProduct(new_product);
		return MATAMIKYA_OUT_OF_MEMORY;
	}

	freeProduct(new_product);
	return MATAMIKYA_SUCCESS;
}

static Product getProductWithID(Set products, unsigned int id)
{
	for (Product prod = setGetFirst(products); prod != NULL; prod = setGetNext(products)) {
		if (compareProductID(prod, id)) {
			///printf("%d\n", getProductID(prod));
			return prod;
		}
	}
	return NULL;
}

static Order getOrderWithID(Set orders, unsigned int id)
{
	if (orders == NULL) {
		return NULL;
	}
	for (Order order = setGetFirst(orders); order != NULL; order = setGetNext(orders)) {
		unsigned int order_id = getOrderID(order);
		if (compareOrder(&order_id, &id) == 0) {
			return order;
		}
	}
	return NULL;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}

	Product product = getProductWithID(matamikya->Products, id);
	if (product == NULL) {
		return MATAMIKYA_PRODUCT_NOT_EXIST;
	}
	if (!isTheAmountTypeLegal(amount, getProductAmountType(product))) {
		return MATAMIKYA_INVALID_AMOUNT;
	}
	if (getProductAmount(product) + amount < 0) {
		return MATAMIKYA_INSUFFICIENT_AMOUNT;
	}

	setProductAmount(product, amount);
	return MATAMIKYA_SUCCESS;
}

static void removeProductFromOrdersByID(Set orders, unsigned int id)
{
	if (orders == NULL) {
		return;
	}
	for (Order order = setGetFirst(orders); order != NULL; order = setGetNext(orders)) {
		removeOrderProductByID(order, id);
	}
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	Product product = getProductWithID(matamikya->Products, id);
	if (product == NULL) {
		return MATAMIKYA_PRODUCT_NOT_EXIST;
	}
	freeProduct(product);
	removeProductFromOrdersByID(matamikya->orders, id);
	return MATAMIKYA_SUCCESS;
}

static unsigned int getMaxOrderID(Set orders)
{
	if (orders == NULL) {
		return 0;
	}
	unsigned int max_product_id = getProductID(setGetFirst(orders));
	for (Product product = setGetFirst(orders); product != NULL; product = setGetNext(orders)) {
		if (max_product_id < getProductID(product)) {
			max_product_id = getProductID(product);
		}
	}
	return max_product_id;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
	if (matamikya == NULL) {
		return 0;
	}
	Order new_order = creatOrder(1);
	if (new_order == NULL) {
		return 0;
	}

	setOrderID(new_order, getMaxOrderID(matamikya->orders) + 1);
	if (setAdd(matamikya->orders, new_order) != SET_SUCCESS) {
		return 0;
	}
	return getOrderID(new_order);
}

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
											  const unsigned int productId, const double amount)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}

	if (getProductWithID(matamikya->Products, productId) == NULL) {
		return MATAMIKYA_PRODUCT_NOT_EXIST;
	}


	Order order = getOrderWithID(matamikya->orders, orderId);
	if (order == NULL) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}

	Product product = getProductWithID(getOrderProducts(order), productId);
	if (!isTheAmountTypeLegal(amount, getProductAmountType(product))) {
		return MATAMIKYA_INVALID_AMOUNT;
	}
	if (product == NULL && amount > 0) {
		setAdd(getOrderProducts(order), product);
		return MATAMIKYA_SUCCESS;
	}

	if (getProductAmount(product) + amount <= 0) {
		removeOrderProductByID(order, productId);
		return MATAMIKYA_SUCCESS;
	}

	setProductAmount(product, amount);
	return MATAMIKYA_SUCCESS;
}

static void SetProfits(Order order, Set products)
{
	SET_FOREACH(Product, product_of_order, getOrderProducts(order)) {
		setProductAmount(getProductWithID(products, getProductID(product_of_order)),
						 -getProductAmount(product_of_order));
	}

}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}

	Order order = getOrderWithID(matamikya->orders, orderId);
	if (order == NULL) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}
	for (Product product_of_order = setGetFirst(getOrderProducts(order));
		 product_of_order != NULL; product_of_order = setGetNext(getOrderProducts(order))) {

		Product product_of_warehouse = getProductWithID(matamikya->Products, getProductID(product_of_order));
		if (getProductAmount(product_of_warehouse) - getProductAmount(product_of_order) < 0) {
			return MATAMIKYA_INSUFFICIENT_AMOUNT;
		}
	}
	SetProfits(order, matamikya->Products);
	setRemove(matamikya->orders, order);
	return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	Order order = getOrderWithID(matamikya->orders, orderId);
	if (order == NULL) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}
	setRemove(matamikya->orders, order);
	return MATAMIKYA_SUCCESS;
}

static Product getFirstSmallProduct(Set products)
{
	/*
	if (products == NULL) {
		return NULL;
	}
	 */
	unsigned int min_product_id = getProductID(setGetFirst(products));
	for (Product product = setGetFirst(products); product != NULL; product = setGetNext(products)) {
		if (getProductID(product) < min_product_id) {
			min_product_id = getProductID(product);
		}
	}
	return getProductWithID(products, min_product_id);
}

static Product getNextSmallProduct(Set products, unsigned int productID)
{

	if (products == NULL) {
		return NULL;
	}
	unsigned int min_product_id;
	for (Product product = setGetFirst(products); product != NULL; product = setGetNext(products)) {
		if (productID < getProductID(product)) {
			min_product_id = getProductID(product);
			break;
		}
	}
	for (Product product = setGetFirst(products); product != NULL; product = setGetNext(products)) {
		if (productID < getProductID(product) && getProductID(product) < min_product_id) {
			min_product_id = getProductID(product);
		}
	}
	return getProductWithID(products, min_product_id);
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE* output)
{
	if (matamikya == NULL || matamikya->Products == NULL || output == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	for (Product product = getFirstSmallProduct(matamikya->Products); product != NULL;
		 product = getNextSmallProduct(matamikya->Products, getProductID(product))) {
		mtmPrintProductDetails(getProductName(product), getProductID(product),
							   getProductAmount(product), getProductPriceOneUnit(product), output);
	}
	return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE* output)
{
	if (matamikya == NULL || output == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	Order order = getOrderWithID(matamikya->orders, orderId);
	if (order == NULL) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}
	mtmPrintOrderHeading(orderId, output);
	for (Product product = getFirstSmallProduct(getOrderProducts(order));
		 product != NULL; product = getNextSmallProduct(getOrderProducts(order),
														getProductID(product))) {
		mtmPrintProductDetails(getProductName(product), getProductID(product),
							   getProductAmount(product), getProductTotalPrice(product), output);
	}
	fprintf(output, "----------\n");
	mtmPrintOrderSummary(getOrderProfits(order), output);
	return MATAMIKYA_SUCCESS;
}

static double maximumInCome(Set products)
{
	double max = 0;
	for (Product product = setGetFirst(products); product != NULL; product = setGetNext(products)) {
		if (getProductTotalInCome(product) > max) {
			max = getProductTotalInCome(product);
		}
	}
	return max;
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE* output)
{
	if (matamikya == NULL || matamikya->Products == NULL || output == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	double max_in_come = maximumInCome(matamikya->Products);
	fprintf(output, "Best Selling Product:\n");
	if (max_in_come == 0) {
		fprintf(output, "none\n");
		return MATAMIKYA_SUCCESS;
	}
	for (Product product = getFirstSmallProduct(matamikya->Products); product != NULL;
		 product = getNextSmallProduct(matamikya->Products, getProductID(product))) {
		if (getProductTotalInCome(product) == max_in_come) {
			mtmPrintIncomeLine(getProductName(product), getProductID(product), getProductTotalInCome(product), output);
			break;
		}
	}
	return MATAMIKYA_SUCCESS;
}
