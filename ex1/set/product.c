//
// Created by 97250 on 02/12/2021.
//

#include "product.h"
#include "string.h"
#include "stdlib.h"
#include "matamikya_print.h"

struct product_t {
	char* name;
	unsigned int productId;
	MatamikyaAmountType amountType;
	MtmProductData productData;
	MtmCopyData copyData;
	MtmFreeData freeData;
	MtmGetProductPrice getProductPrice;

};


void* copyProduct(void* element)
{
	if (element == NULL) {
		return NULL;
	}
	Product product = element;
	Product new_product = creatProduct(product->name,
									   product->productId,
									   product->amountType,
									   product->productData,
									   product->copyData,
									   product->freeData,
									   product->getProductPrice);
	if (new_product == NULL) {
		return NULL;
	}
	new_product->productData = copyProduct(product->productData);
	if (new_product->productData == NULL) {
		freeProduct(new_product);
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
	((Product) product)->freeData(((Product) product)->productData);
	free(product);
}

int compareProduct(void* first_product, void* second_product)
{
	if (first_product == NULL || second_product == NULL) {
		return 0;
	}
	Product first_prod = first_product;
	Product second_prod = second_product;
	return first_prod->productId - second_prod->productId;
}

Product creatProduct(const char* name,
					 unsigned int productId,
					 MatamikyaAmountType amountType,
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
	new_product->name = malloc(strlen(name) + 1);
	if (new_product->name == NULL) {
		return NULL;
	}
	strcpy(new_product->name, name);
	new_product->productId = productId;
	new_product->amountType = amountType;
	new_product->productData = productData;
	new_product->copyData = copyData;
	new_product->freeData = freeData;
	new_product->getProductPrice = getProductPrice;
	return new_product;
}


bool compareProductID(Product prod, int id)
{
	if (prod->productId == id) {
		return true;
	}
	return false;
}

void printProductDetails(Product product, FILE* output)
{
	mtmPrintProductDetails(product->name, product->productId,)
}

void getProductName(Product product)
{

}
