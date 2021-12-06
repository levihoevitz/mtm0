//
// Created by 97250 on 02/12/2021.
//

#include "product.h"
#include "string.h"
#include "stdlib.h"

struct product_t {
	char* name;
	unsigned int productId;
	MatamikyaAmountType amountType;
	double amount;
	MtmProductData productData;
	MtmCopyData copyData;
	MtmFreeData freeData;
	MtmGetProductPrice getProductPrice;

};


void* copyProduct(void* product)
{
	if (product == NULL) {
		return NULL;
	}
	Product new_product = creatProduct(((Product) product)->productId,
									   ((Product) product)->name,
									   ((Product) product)->amount);
	if (new_product == NULL) {
		return NULL;
	}
	return new_product;
}

void freeProduct(void* product)
{
	if (product == NULL) {
		return;
	}
	free(((Product) product)->name);
	free(product);
}

int compareProduct(void* first_product, void* second_product)
{
	if (first_product == NULL || second_product == NULL) {
		return 0;
	}
	///complete
	return 1;
}

Product creatProduct(const char* name,
					 unsigned int productId,
					 MatamikyaAmountType amountType,
					 double amount,
					 MtmProductData productData,
					 MtmCopyData copyData,
					 MtmFreeData freeData,
					 MtmGetProductPrice getProductPrice)
{
	if (name == NULL) {
		return NULL;
	}
	Product new_product = malloc(sizeof(new_product));
	if (new_product == NULL) {
		return NULL;
	}
	strcpy(new_product->name, name);
	new_product->amount = amount;
	new_product->productId = id;
	return new_product;
}


MatamikyaAmountType amountType;
MtmProductData customData;
MtmCopyData copyData;
MtmFreeData freeData;
MtmGetProductPrice prodPrice;

