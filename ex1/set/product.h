//
// Created by 97250 on 02/12/2021.
//

#ifndef MTM0_PRODUCT_H
#define MTM0_PRODUCT_H
#include "stdbool.h"
typedef struct product_t* Product;

typedef void* MtmProductData;

typedef enum MatamikyaAmountType_t {
	MATAMIKYA_INTEGER_AMOUNT,
	MATAMIKYA_HALF_INTEGER_AMOUNT,
	MATAMIKYA_ANY_AMOUNT,
} MatamikyaAmountType;


/**
 * Type of function for copying a product's custom element.
 *
 * Such a function receives a MtmProductData, creates a new MtmProductData
 * object, and returns a pointer to the new object. If the function fails for
 * any reason, it returns NULL.
 */
typedef MtmProductData (* MtmCopyData)(MtmProductData);

/**
 * Type of function for freeing a product's custom element.
 *
 * Such a function receives a MtmProductData object and frees it. The
 * MtmProductData can be NULL, in which case the function does nothing.
 */
typedef void (* MtmFreeData)(MtmProductData);

/**
 * Type of function for calculating the price of a product.
 *
 * Such a function receives the product's custom element (a MtmProductData) and an
 * amount (a double), and returns the price (a double) of purchasing the given
 * quantity of the given product.
 *
 * For example, a function that treats MtmProductData as the price of a single
 * item, with no discounts or other special considerations:
 * @code
 * double basicGetPrice(MtmProductData basePrice, double amount) {
 *     return (*(double*)basePrice) * amount;
 * }
 * @endcode
 */
typedef double (* MtmGetProductPrice)(MtmProductData, const double amount);

Product creatProduct(const char* name,
					 unsigned int productId,
					 MatamikyaAmountType amountType,
					 MtmProductData productData,
					 MtmCopyData copyData,
					 MtmFreeData freeData,
					 MtmGetProductPrice getProductPrice);

void* copyProduct(void* product);

void freeProduct(void* product);

int compareProduct(void* first_product, void* second_product);

bool compareProductID(Product prod, int id);
#endif //MTM0_PRODUCT_H
