#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr;

    // malloc()
    arr = (int *)malloc(5 * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Memory allocated using malloc()\n");

    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
    }

    printf("Values after malloc: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // realloc()
    arr = (int *)realloc(arr, 10 * sizeof(int));

    if (arr == NULL) {
        printf("Memory reallocation failed\n");
        return 1;
    }

    printf("Memory resized using realloc()\n");

    for (int i = 5; i < 10; i++) {
        arr[i] = (i + 1) * 10;
    }

    printf("Values after realloc: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // calloc()
    int *zeros = (int *)calloc(5, sizeof(int));

    if (zeros == NULL) {
        printf("Calloc allocation failed\n");
        free(arr);
        return 1;
    }

    printf("Memory allocated using calloc(): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", zeros[i]);
    }
    printf("\n");

    // free()
    free(arr);
    free(zeros);

    printf("Memory freed successfully\n");

    return 0;
}
