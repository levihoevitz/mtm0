#include <stdio.h>
#include <stdbool.h>

int getTheSizeOfTheArray()
{
	printf("Enter size of input:");
	int size;
	if (scanf("%d", &size) == 0 || size <= 0) {
		printf("%d",size);
		printf("Invalid size");
		return 0;
	}
	return size;
}

bool getTheNumForTheArray(int arr[], int size)
{
	int num;
	for (int i = 0; i < size; ++i) {
		if (scanf("%d", &num)  && num >0){
			arr[i] = num;
		} else {
			printf("Invalid size");
			return false;
		}
	}
	return true;
}

int calculateIfTheNumberIsPowerOfTwo(int num)
{
	int power = 0;
	if (num == 1) {
		return power;
	}
	while (num != 1) {
		if (num % 2 != 0) {
			return -1;
		}
		num /= 2;
		power++;
	}
	return power;
}

void checkAndPrintThePowerOfTwo(int arr[], int size)
{
	int power;
	for (int i = 0; i < size; ++i) {
		power = calculateIfTheNumberIsPowerOfTwo(arr[i]);
		if (power != -1) {
			printf("The number is a power of 2: %d = 2^%d\n", arr[i], power);
		}
	}
}


int main()
{
	int size = getTheSizeOfTheArray();
	if(size==0){
		return 0;
	}
	int arrayOfNum[size];
	if(!getTheNumForTheArray(arrayOfNum, size)){
		return 0;
	}
	checkAndPrintThePowerOfTwo(arrayOfNum, size);
	return 0;
}
