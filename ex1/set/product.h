#ifndef MTM0_PRODUCT_H
#define MTM0_PRODUCT_H
#include "stdbool.h"
#include "matamikya.h"

typedef struct product_t* Product;

/**
 * creatProduct: add a new product to a Matamikya warehouse.
 *
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
 *     NULL -  if name/productData/copyData/freeData/newProduct are Null.
 *     new_product - if the creation is successfull.
 */
Product creatProduct(const char* name,
					 unsigned int productId,
					 double amount,
					 MatamikyaAmountType amountType,
					 MtmProductData productData,
					 MtmCopyData copyData,
					 MtmFreeData freeData,
					 MtmGetProductPrice getProductPrice);

/**
 * copyProduct: copies product.
 *
 * @param element - Generic pointer to a product.
 * @return
 *     NULL -  if element/newProduct are Null.
 *     new_product - if copies product successfully.
 */
void* copyProduct(void* element);

/**
 * freeProduct: frees product.
 *
 * @param element - Generic pointer to a product.
 * @return
 *     NULL -  if element is Null.
 *     void - if frees product successfully.
 */
void freeProduct(void* element);

/**
 * compareProduct: compares products.
 *
 * @param first_product - pointer to the first product.
 * @param second_product - pointer to the second product.

 * @return
 *     0 -  if the first and second product are the same product.
 *     1 - if the first product comes before the second product.
 *     -1 - if the first product comes after the second product.

 */
int compareProduct(void* first_product, void* second_product);

/**
 * compareProductID: checkes if the given ID matches the given product.
 *
 * @param prod - a given product.
 * @param id - a given ID.

 * @return
 *     true -  if the given ID is indeed the ID of the given product.
 *     false - if the given ID doesn't match the ID of the given product.
 */
bool compareProductID(Product prod,unsigned int id);

/**
 * getProductName: Gets the name of a given product
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     prouct.name - if product exists.
 */
char* getProductName(Product product);

/**
 * getProductID: Gets the ID of a given product
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     prouct.productId - if product exists.
 */
unsigned int getProductID(Product product);

/**
 * getProductAmountType: Checks the amount type of the product.
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     prouct.amountType - if product exists.
 */
MatamikyaAmountType getProductAmountType(Product product);

/**
 * getProductAmount: Gets the amount of the product.
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     prouct.amount - if product exists.
 */
double getProductAmount(Product product);

/**
 * getProductTotalPrice: Gets the total price of the product.
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     product->getProductPrice(product->productData, product->amount) - if product exists.
 */
double getProductTotalPrice(Product product);

/**
 * getProductPriceOneUnit: Gets the price of one unit of the product.
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     product->getProductPrice(product->productData, 1) - if product exists.
 */
double getProductPriceOneUnit(Product product);

/**
 * getProductTotalInCome: Gets how much profit the product made.
 *
 * @param product - a given product.

 * @return
 *     Null - if product is Null.
 *     product->totalInCome - if product exists.
 */
double getProductTotalInCome(Product product);

/**
 * setProductAmount: Upgrades the quantity of the product.
 *
 * @param product - a given product.
 * @param amount - the new stock amount that has been recieved of the product.

 * @return
 *     Null - if product is Null.
 *     void - if upgraded successfully.
 */
void setProductAmount(Product product, double amount);

void addProductAmount(Product product, double amount);

#endif //MTM0_PRODUCT_H
