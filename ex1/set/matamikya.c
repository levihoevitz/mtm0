#include "matamikya.h"
#include <stdlib.h>
#include "set.h"
#include "product.h"
#include "string.h"
#include "amount_set.h"

struct Matamikya_t {
	AmountSet Products;
	Set orders;
};


struct order_t {
	unsigned int id;
	char* name;
	double amount;
	//להוסיף רווח
};

/**
 * matamikyaCreate: create an empty Matamikya warehouse.
 *
 * @return A new Matamikya warehouse in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Matamikya matamikyaCreate()
{
	Matamikya new_matamikya = malloc(sizeof(new_matamikya));
	if (new_matamikya == NULL) {
		return NULL;
	}
	new_matamikya->Products = asCreate(copyProduct,
									   freeProduct,
									   compareProduct);
	if (new_matamikya->Products == NULL) {
		free(new_matamikya);
		return NULL;
	}
	///to change function
	new_matamikya->orders = setCreate(copyProduct,
									  freeProduct,
									  compareProduct);
	if (new_matamikya->Products == NULL) {
		setDestroy(new_matamikya->orders);
		free(new_matamikya);
		return NULL;
	}
	return new_matamikya;
}

/**
 * matamikyaDestroy: free a Matamikya warehouse, and all its contents, from
 * memory.
 *
 * @param matamikya - the warehouse to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void matamikyaDestroy(Matamikya matamikya)
{
	if (matamikya == NULL) {
		return;
	}
	asDestroy(matamikya->Products);
	setDestroy(matamikya->orders);
	free(matamikya);
}

/**
 * mtmNewProduct: add a new product to a Matamikya warehouse.
 *
 * @param matamikya - warehouse to add the product to. Must be non-NULL.
 * @param id - new product productId. Must be non-negative, and unique.
 * @param name - name of the product, e.g. "apple". Must be non-empty.
 * @param amount - the initial amount of the product when added to the warehouse.
 * @param amountType - defines what are valid amounts for this product.
 * @param customData - pointer to product's additional element
 * @param copyData - function pointer to be used for copying product's additional
 *      element.
 * @param freeData - function pointer to be used for free product element.
 * @param prodPrice - function pointer to be used for getting the price of some
 *      product.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if matamikya/name/customData/copyData/freeData
 *      /prodPrice are NULL.
 *     MATAMIKYA_INVALID_NAME - if name is empty, or doesn't start with a
 *         letter (a -z, A -Z) or a digit (0 -9).
 *     MATAMIKYA_INVALID_AMOUNT - if amount < 0, or is not consistent with amountType
 *         (@see MatamikyaAmountType documentation above)
 *     MATAMIKYA_PRODUCT_ALREADY_EXIST - if a product with the given productId already exist.
 *     MATAMIKYA_SUCCESS - if product was added successfully.
 */
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

static bool isTheAmountLegal(const double amount)
{
	if (amount < 0) {
		return false;
	}
	///check type
	return true;
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
	if (!isTheAmountLegal(amount)) {
		return MATAMIKYA_INVALID_AMOUNT;
	}
	///check it
	if (asContains(matamikya->Products, &id)) {
		return MATAMIKYA_PRODUCT_ALREADY_EXIST;
	}

	Product new_product = creatProduct(name, id, amountType, amount,
									   customData, copyData, freeData, prodPrice);
	if(new_product==NULL){
		return MATAMIKYA_OUT_OF_MEMORY;
	}
	if(asRegister(matamikya->Products,new_product)!=AS_SUCCESS){
		free(new_product);
		return MATAMIKYA_OUT_OF_MEMORY;
	}
	free(new_product);
	return MATAMIKYA_SUCCESS;
}

/**
 * mtmChangeProductAmount: increase or decrease the amount of an *existing* product in a Matamikya warehouse.
 * if 'amount' < 0 then this amount should be decreased from the matamikya warehouse.
 * if 'amount' > 0 then this amount should be added to the matamikya warehouse.
 * if 'amount' = 0 then nothing should be done.
 * please note:
 * If the amount to decrease is larger than the product's amount in the
 * warehouse, then the product's amount is not changed, and a proper error-code
 * is returned.
 * If the amount is equal to the product's amount in the
 * warehouse,then the product will remain inside the warehouse
 * with amount of zero.
 *
 * @param matamikya - warehouse to add the product to. Must be non-NULL.
 * @param id - existing product productId. Must exist in the warehouse.
 * @param amount - the amount of the product to increase/decrease to the warehouse.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if matamikya does not contain a product with
 *         the given productId.
 *     MATAMIKYA_INVALID_AMOUNT - if amount is not consistent with product's amount type
 *         (@see parameter amountType in mtmNewProduct).
 *     MATAMIKYA_INSUFFICIENT_AMOUNT - if 'amount' < 0 and the amount to be decreased
 *         is bigger than product's amount in the warehouse.
 *     MATAMIKYA_SUCCESS - if product amount was increased/decreased successfully.
 * @note Even if amount is 0 (thus the function will change nothing), still a proper
 *    error code is returned if one of the parameters is invalid, and MATAMIKYA_SUCCESS
 *    is returned if all the parameters are valid.
 */
MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	if (amount) {
		return MATAMIKYA_INSUFFICIENT_AMOUNT;
	}
	if (amount) {
		return MATAMIKYA_INVALID_AMOUNT;
	}
	if (id) {
		return MATAMIKYA_PRODUCT_NOT_EXIST;
	}

	return MATAMIKYA_SUCCESS;
}

/**
 * mtmClearProduct: clear a product from a Matamikya warehouse.
 *
 * The entire amount of the product is removed, and the product is erased
 * completely from the warehouse, from all existing orders and from the
 * 'income' mechanism(holding the profits for each existing product).
 * For example, after clearing a product with
 * mtmClearProduct, calling mtmChangeProductAmount on that product will fail.
 *
 * @param matamikya - warehouse to remove the product from.
 * @param id - productId of product to be removed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if matamikya does not contain a product with
 *         the given productId.
 *     MATAMIKYA_SUCCESS - if product was cleared successfully.
 */
MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}

	if (id) {
		return MATAMIKYA_PRODUCT_NOT_EXIST;
	}

	return MATAMIKYA_SUCCESS;
}

/**
 * mtmCreateNewOrder: create a new empty order in a Matamikya warehouse, and
 * return the order's productId.
 *
 * @param matamikya - a Matamikya warehouse
 * @return
 *     Positive productId of the new order, if successful.
 *     0 in case of failure.
 */
unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
	if (matamikya == NULL) {
		return 0;
	}
	return 1;
}

/**
 * mtmChangeProductAmountInOrder: add/increase/remove/decrease products to an existing order.
 * Only products that exist inside the matamikya can be added to an order.
 * if 'amount' < 0 then this amount should be decreased from the product in the order.
 * if 'amount' > 0 then this amount should be added to the product in the order.
 * if 'amount' = 0 then nothing should be done.
 * please note:
 *  1) If the amount to decrease('amount' < 0) is *larger or equal* then the amount of the product in the
 *     order, then the product is removed entirely from the order.
 *  2) If 'amount' > 0 and the product doesn't exist inside the order then you should add it to the order
 *     with the amount given as argument.
 *
 * @param matamikya - warehouse containing the order and the product.
 * @param orderId - productId of the order being modified.
 * @param productId - productId of product to add to the order.
 * @param amount - amount of product to add to the order.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_PRODUCT_NOT_EXIST - if matamikya does not contain a product with
 *         the given productId.
 *     MATAMIKYA_INVALID_AMOUNT - if amount is not consistent with product's amount type
 *         (@see parameter amountType in mtmNewProduct).
 *     MATAMIKYA_SUCCESS - if product was added/removed/increased/decreased to the order successfully.
 * @note Even if amount is 0 (thus the function will change nothing), still a proper
 *    error code is returned if one of the parameters is invalid, and MATAMIKYA_SUCCESS
 *    is returned if all the parameters are valid.
 */
MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
											  const unsigned int productId, const double amount)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	if (orderId) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}
	if (productId) {
		return MATAMIKYA_PRODUCT_NOT_EXIST;
	}
	if (amount) {
		return MATAMIKYA_INVALID_AMOUNT;
	}


	return MATAMIKYA_SUCCESS;
}

/**
 * mtmShipOrder: ship an order and remove it from a Matamikya warehouse.
 *
 * All products in the order are removed from the warehouse, and the order is
 * deleted. The amount of each product in the order is the amount of the product
 * that is removed from the warehouse. additionally once order is shipped
 * the profit from the products shipped needs to be updated
 *
 * If the order cannot be shipped for any reason, e.g. some product's amount in
 * the order is larger than its amount in the warehouse, then the entire
 * operation is canceled - the order remains in the warehouse, and the
 * warehouse contents are not modified.
 *
 * @param matamikya - warehouse containing the order and all the products.
 * @param orderId - productId of the order being shipped.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_INSUFFICIENT_AMOUNT - if the order contains a product with an amount
 *         that is larger than its amount in matamikya.
 *     MATAMIKYA_SUCCESS - if the order was shipped successfully.
 */
MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	if (orderId) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}
	if (1) {
		return MATAMIKYA_INSUFFICIENT_AMOUNT;
	}
	return MATAMIKYA_SUCCESS;
}

/**
 * mtmCancelOrder: cancel an order and remove it from a Matamikya warehouse.
 *
 * The order is deleted from the warehouse. The products and their amounts in
 * the warehouse is not changed.
 *
 * @param matamikya - warehouse containing the order.
 * @param orderId - productId of the order being canceled.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_SUCCESS - if the order was shipped successfully.
 */
MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
	if (matamikya == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	if (orderId) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}

	return MATAMIKYA_SUCCESS;
}

/**
 * mtmPrintInventory: print a Matamikya warehouse and its contents as
 * explained in the *.pdf
 *
 * @param matamikya - a Matamikya warehouse to print.
 * @param output - an open, writable output stream, to which the contents are printed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_SUCCESS - if printed successfully.
 */
MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE* output)
{
	if (matamikya == NULL || output == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}

	return MATAMIKYA_SUCCESS;
}

/**
 * matamikyaPrintOrder: print a summary of an order from a Matamikya warehouse,
 * as explained in the *.pdf
 *
 * The printout includes the total price of the order.
 *
 * @param matamikya - the Matamikya warehouse containing the order.
 * @param orderId - productId of the order in matamikya.
 * @param output - an open, writable output stream, to which the order is printed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_ORDER_NOT_EXIST - if matamikya does not contain an order with
 *         the given orderId.
 *     MATAMIKYA_SUCCESS - if printed successfully.
 */
MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE* output)
{
	if (matamikya == NULL || output == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}
	if (orderId) {
		return MATAMIKYA_ORDER_NOT_EXIST;
	}

	return MATAMIKYA_SUCCESS;
}

/**
 * mtmPrintBestSelling: print the best selling products of a Matamikya
 * warehouse, as explained in the *.pdf.
 *
 * @param matamikya - a Matamikya warehouse.
 * @param output - an open, writable output stream, to which the order is printed.
 * @return
 *     MATAMIKYA_NULL_ARGUMENT - if a NULL argument is passed.
 *     MATAMIKYA_SUCCESS - if printed successfully.
 */
MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE* output)
{
	if (matamikya == NULL || output == NULL) {
		return MATAMIKYA_NULL_ARGUMENT;
	}


	return MATAMIKYA_SUCCESS;
}
