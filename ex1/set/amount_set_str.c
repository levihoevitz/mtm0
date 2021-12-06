//
// Created by 97250 on 27/11/2021.
//

#include "amount_set_str.h"
//#include "amount_set.h"
#include <stdlib.h>
#include <string.h>

typedef struct nod_t {
	char* element;
	double amount;
	struct nod_t* next;
} * Node;

struct AmountSet_t {
	Node head;
	Node iterator;
	int size;
};

/**
 * asCreate: Allocates a new empty amount set.
 *
 * @return
 *     NULL - if one of the parameters is NULL or allocations failed.
 *     A new amount set in case of success.
 */
AmountSet asCreate()
{
	AmountSet new_as = malloc(sizeof(new_as));
	if (new_as == NULL) {
		return NULL;
	}
	new_as->head = NULL;
	new_as->iterator = NULL;
	new_as->size = 0;
	return new_as;
}

/**
 * asDestroy: Deallocates an existing amount set. Clears all elements.
 *
 * @param set - Target set to be deallocated. If set is NULL nothing will be done.
 */
void asDestroy(AmountSet set)
{
	if (set == NULL) {
		return;
	}
	asClear(set);
	free(set);
}

/**
 * asCopy: Creates a copy of target set.
 *
 * Iterator values for both sets are undefined after this operation.
 *
 * @param set - Target set.
 * @return
 *     NULL if a NULL was sent or a memory allocation failed.
 *     An amount set containing the same elements (and amounts) as set, otherwise.
 */
AmountSet asCopy(AmountSet set)
{
	if (set == NULL) {
		return NULL;
	}
	AmountSet copy_set = asCreate();
	if (copy_set == NULL) {
		return NULL;
	}
	for (Node i = set->head; i != NULL; i = i->next) {
		if (asRegister(copy_set, i->element) != AS_SUCCESS) {
			free(copy_set);
			return NULL;
		}
		asChangeAmount(copy_set, i->element, i->amount);
	}
	return copy_set;
}

/**
 * asGetSize: Returns the number of elements in a set.
 *
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The set which size is requested.
 * @return
 *     -1 if a NULL pointer was sent.
 *     Otherwise the number of elements in the set.
 */
int asGetSize(AmountSet set)
{
	if (set == NULL) {
		return -1;
	}
	return set->size;
}

/**
 * asContains: Checks if an element exists in the set.
 *
 * The element will be considered in the set if one of the elements in the set
 * is determined equal (equality between strings).
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The set to search in.
 * @param element - The element to look for.
 * @return
 *     false - if the input set is null, or if the element was not found.
 *     true - if the element was found in the set.
 */
bool asContains(AmountSet set, const char* element)
{
	// need to check if element can be NULL
	if (set == NULL || element == NULL) {
		return false;
	}
	for (Node i = set->head; i != NULL; i = i->next) {
		if (strcmp(i->element,element)==0) {
			return true;
		}
	}
	return false;
}

/**
 * asGetAmount: Returns the amount of an element in the set.
 *
 * The function returns an error code indicating wether the operation succeeded,
 * and in case of success also returns the element's amount via the outAmount
 * pointer.
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The set which contains the element.
 * @param element - The element whose amount is requested.
 * @param outAmount - Pointer to the location where the amount is returned, in case
 *     of success. In case of failure, the contents of outAmount are unchanged.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *     AS_SUCCESS - if the amount was returned successfully.
 *
 * For example, to print the amount of the first item in the set:
 * @code
 * double firstAmount;
 * char* first = asGetFirst(set);
 * if (asGetAmount(set, first, &firstAmount) == AS_SUCCESS) {
 *     printf("First amount: %f\n", firstAmount);
 * }
 * @endcode
 */
AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
	if (set == NULL || element == NULL) {
		return AS_NULL_ARGUMENT;
	}
	if (!asContains(set, element)) {
		return AS_ITEM_DOES_NOT_EXIST;
	}
	for (Node i = set->head; i != NULL; i = i->next) {
		if (strcmp(i->element, element)) {
			outAmount = &i->amount;
			return AS_SUCCESS;
		}
	}
	return AS_SUCCESS;
}

/**
 * asRegister: Add a new element into the set.
 *
 * The element is added with an initial amount of 0.
 * Iterator's value is undefined after this operation.
 *
 * @param set - The target set to which the element is added.
 * @param element - The element to add.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_ALREADY_EXISTS - if an equal element already exists in the set.
 *     AS_SUCCESS - if the element was added successfully.
 */

AmountSetResult asRegister(AmountSet set, const char* element)
{
	if (set == NULL || element == NULL) {
		return AS_NULL_ARGUMENT;
	}
	if (asContains(set, element)) {
		return AS_ITEM_ALREADY_EXISTS;
	}
	Node new_nod = malloc(sizeof(new_nod));
	if (new_nod == NULL) {
		return AS_OUT_OF_MEMORY;
	}
	new_nod->element = malloc(strlen(element) + 1);
	if (new_nod->element == NULL) {
		free(new_nod);
		return AS_OUT_OF_MEMORY;
	}
	strcpy(new_nod->element,element);
	new_nod->amount = 0;

///check if the empty set
	if (set->head == NULL) {
		set->head = new_nod;
		set->size++;
		return AS_SUCCESS;
	}
///check if the element the smallest
	if (strcmp(element, set->head->element) > 0) {
		new_nod->next = set->head;
		set->head = new_nod;
		set->size++;
		return AS_SUCCESS;
	}
///check if the element put in the middle
	Node previous_node = NULL;
	for (Node i = set->head->next; i != NULL; i = i->next) {
		if (strcmp(element, i->element) > 0) {
			previous_node->next = new_nod;
			new_nod->next = i;
			set->size++;
			return AS_SUCCESS;
		}
		previous_node = i;
	}
///check if the element in the end
	previous_node->next = new_nod;
	new_nod->next = NULL;
	set->size++;
	return AS_SUCCESS;
}

/**
 * asChangeAmount: Increase or decrease the amount of an element in the set.
 *
 * Iterator's state is unchanged after this operation.
 *
 * @param set - The target set containing the element.
 * @param element - The element whose amount is increased.
 * @param amount - How much to change the element's amount. A positive value
 *     increases element's amount, while a negative value decreases it. A value
 *     of 0 means don't change it.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *     AS_INSUFFICIENT_AMOUNT - if amount is negative and the element's amount
 *         in the set is less than the amount that needs to be decreased (i.e.,
 *         if the change will result in a negative amount for the element in the
 *         set.)
 *     AS_SUCCESS - if the element's amount was changed successfully.
 *
 * @note parameter amount doesn't affect the return value. Even if amount is 0,
 *     still AS_ITEM_DOES_NOT_EXIST is returned if element doesn't exist in set,
 *     and AS_SUCCESS is returned if element exists in set (assuming set is not
 *     NULL).
 */
AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
{
	if (set == NULL || element == NULL) {
		return AS_NULL_ARGUMENT;
	}
	if (!asContains(set, element)) {
		return AS_ITEM_DOES_NOT_EXIST;
	}
	Node i = set->head->next;
	for (; i != NULL; i = i->next) {
		if (strcmp(element, i->element) == 0) {
			i->amount += amount;
			break;
		}
	}
	if (i->amount < 0) {
		i->amount -= amount;
		return AS_INSUFFICIENT_AMOUNT;
	} else {
		return AS_SUCCESS;
	}
}

/**
 * asDelete: Delete an element completely from the set.
 *
 * This is the opposite of asRegister. The element is deallocated.
 * Iterator's value is undefined after this operation.
 *
 * @param set - The target set from which the element is deleted.
 * @param element - The element to delete.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_DOES_NOT_EXIST - if the element doesn't exist in the set.
 *     AS_SUCCESS - if the element was deleted successfully.
 */
AmountSetResult asDelete(AmountSet set, const char* element)
{
	if (set == NULL || element == NULL) {
		return AS_NULL_ARGUMENT;
	}
	if (!asContains(set, element)) {
		return AS_ITEM_DOES_NOT_EXIST;
	}
	Node previous_node = NULL;
	if (strcmp(element, set->head->element) == 0) {
		previous_node = set->head;
		set->head = set->head->next;
		free(previous_node->element);
		free(previous_node);
	} else {
		for (Node i = set->head->next; i != NULL; i = i->next) {
			if (strcmp(element, i->element) == 0) {
				previous_node->next = i->next;
				free(i->element);
				free(i);
				break;
			}
			previous_node = i;
		}
	}
	set->iterator = NULL;
	set->size--;
	return AS_SUCCESS;
}

/**
 * asClear: Deletes all elements from target set.
 *
 * The elements are deallocated.
 * Iterator's value is undefined after this operation.
 *
 * @param set - Target set to delete all elements from.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL pointer was sent.
 *     AS_SUCCESS - Otherwise.
 */
AmountSetResult asClear(AmountSet set)
{
	if (set == NULL) {
		return AS_NULL_ARGUMENT;
	}
	for (Node i = set->head->next; i != NULL; i = i->next) {
		asDelete(set, i->element);
	}
	set->iterator = NULL;
	return AS_SUCCESS;
}

/**
 * asGetFirst: Sets the internal iterator (also called current element) to
 * the first  element in the set. The first element is the smallest element
 * of the set (alphabetically).
 * Use this to start iterating over the set.
 * To continue iteration use asGetNext.
 *
 * @param set - The set for which to set the iterator and return the first
 *     element itself, and not a copy of it.
 * @return
 *     NULL if a NULL pointer was sent or the set is empty.
 *     The first element of the set otherwise
 */
char* asGetFirst(AmountSet set)
{
	if (set == NULL || set->head == NULL) {
		return NULL;
	}
	set->iterator = set->head;
	return set->iterator->element;
}

/**
 * asGetNext: Advances the set iterator to the next element and returns it.
 * The iteration is in ascending order on the set's elements.
 *
 * @param set - The set for which to advance the iterator
 * @return
 *     NULL if reached the end of the set, or the iterator is at an invalid state
 *     or a NULL sent as argument
 *     The next element on the set in case of success
 */
char* asGetNext(AmountSet set)
{
	if (set == NULL || set->iterator == NULL || set->iterator->next == NULL) {
		return NULL;
	}
	set->iterator = set->iterator->next;
	return set->iterator->element;
}


