#ifndef MTM0_PRODUCT_H
#define MTM0_PRODUCT_H
#include "stdbool.h"
#include "matamikya.h"
typedef struct product_t* Product;

Product creatProduct(const char* name,
					 unsigned int productId,
					 double amount,
					 MatamikyaAmountType amountType,
					 MtmProductData productData,
					 MtmCopyData copyData,
					 MtmFreeData freeData,
					 MtmGetProductPrice getProductPrice);

void* copyProduct(void* element);

void freeProduct(void* element);

int compareProduct(void* first_product, void* second_product);

bool compareProductID(Product prod,unsigned int id);

char* getProductName(Product product);

unsigned int getProductID(Product product);

MatamikyaAmountType getProductAmountType(Product product);

double getProductAmount(Product product);

double getProductTotalPrice(Product product);

double getProductPriceOneUnit(Product product);

double getProductTotalInCome(Product product);

void setProductAmount(Product product, double amount);

#endif //MTM0_PRODUCT_H
