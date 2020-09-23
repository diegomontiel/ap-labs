#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void merge(void *arrayList[], int l, int m, int r, int(*comp)(void*, void*)) {
	int i;
	int ls = m-l+1;
	int rs = r-m;
	void *left[ls];
	void *right[rs];

	for(i=0; i<ls; i++) {
		left[i] = arrayList[l+1];
	}
	for(i=0; i<rs; i++) {
		right[i] = arrayList[m+i+1];
	}

	int j = 0;
	int k = l;
	i = 0;

	while(i<ls && j<rs) {
		if(comp(left[i],right[j]) < 0) {
			arrayList[k++] = left[i++];
		} else {
			arrayList[k++] = right[j++];
		}
	}
	
	if(i==ls) {
		while(j<rs) {
			arrayList[k++] = right[j++];
		}
	} else {
		while(i<ls) {
			arrayList[k++] = left[i++];
		}
	}
}

void mergeSort(void *arrayList[], int first, int last, int (*comp)(void*, void*)) {
	int mid = first +  (last-first)/2;
	if(first<last){
		mergeSort(arrayList, first, mid, comp);
		mergeSort(arrayList, mid+1, last, comp);
		merge(arrayList, first, mid, last, comp);
	}
}

int numcmp(char *left, char *right) {
	float n1 = atof(left);
	float n2 = atof(right);
	if(n1<n2)
		return -1;
	if(n1==n2)
		return 0;
	return 1;
}

void printArray(char *arr[], int size) {
	for(int i = 0; i<size; i++) {
		printf("%s ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	char *input;
	bool isnum;

	if(argc == 2) {
		input = argv[1];
		isnum = false;
	} else if (argc == 3) {
		input = argv[2];
		isnum = true;
	} else {
		printf("Missing parameters!!\n");
		return 0;
	}

	int arraySize = 100;
	char **arrayList = malloc(sizeof(char *) *  arraySize);

	FILE *fEntry = fopen(input, "r");
	if(!fEntry){
		printf("Incomplete parameters!!\n");
		return 0;
	}

	char *buffer = NULL;
	size_t bufferSize = 0;
	int counter = 0;
	ssize_t lineSize;

	lineSize = getline(&buffer, &bufferSize, fEntry);
	while(lineSize >= 0){
		arrayList[counter] = malloc(sizeof(char) * bufferSize);

		sprintf(arrayList[counter], "%s", buffer);
		arrayList[counter][strlen(buffer) - 1] = 0;

		counter++;
		lineSize = getline(&buffer, &bufferSize, fEntry);
	}
	free(buffer);
	fclose(fEntry);
	if(isnum){
		mergeSort((void *) arrayList, 0, arraySize - 1, (int (*)(void*, void*)) numcmp);
		printArray(arrayList, arraySize);
	} else {
		mergeSort((void *) arrayList, 0, arraySize - 1, (int (*)(void*, void*)) strcmp);
		printArray(arrayList, arraySize);
	}
	return 0;
}
