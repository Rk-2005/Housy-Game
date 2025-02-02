#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void rmv(int a[], int index);
void display(int arr[], int n, char var);
int iscomplete(int a[], int n);
int search(int a[], int n, int ele);

int main() {

    FILE *fpa;

    // Opening first file of A
    fpa = fopen("A.txt", "r");
    int sizea = 0; // Size of array of A
    int arrA[10];

    // Extracting data from file to array
    if (fpa == NULL) {
        printf("Couldn't open judge file\n");
    } else {
        int i = 0;
        int buff;
        while (fscanf(fpa, "%d ", &buff) == 1) {
            arrA[i] = buff;
            sizea++;
            i++;
        }
        arrA[sizea] = -1; // Last element to -1 for end
    }

    // B
    FILE *fpb;
    fpb = fopen("B.txt", "r");
    int sizeb = 0;
    int arrB[10];

    if (fpb == NULL) {
        printf("Couldn't open judge file\n");
    } else {
        int i = 0;
        int buff;
        while (fscanf(fpb, "%d ", &buff) == 1) {
            arrB[i] = buff;
            sizeb++;
            i++;
        }
        arrB[sizeb] = -1; // Last element to -1 for end
    }

    // C
    FILE *fpc;
    fpc = fopen("C.txt", "r");
    int sizec = 0;
    int arrC[10];

    if (fpc == NULL) {
        printf("Couldn't open judge file\n");
    } else {
        int i = 0;
        int buff;
        while (fscanf(fpc, "%d ", &buff) == 1) {
            arrC[i] = buff;
            sizec++;
            i++;
        }
        arrC[sizec] = -1; // Last element to -1 for end
    }

    // Shared part
    key_t key1 = 5000; // Key for current
    key_t key2 = 6000; // Key for synchronization
    key_t key3 = 7000; // Key for terminate

    int shmid1 = shmget(key1, 4, 0666 | IPC_CREAT);      // Segment for current
    int shmid2 = shmget(key2, 4, 0666 | IPC_CREAT);      // Segment for synchronization
    int shmid3 = shmget(key3, 4, 0666 | IPC_CREAT);      // Segment for termination
    int *terminate = (int *)shmat(shmid3, (void *)0, 0); // For synchronization element in judge
    *terminate = 0;

    int *current = (int *)shmat(shmid1, (void *)0, 0);         // For current element in judge
    int *synchronization = (int *)shmat(shmid2, (void *)0, 0); // For synchronization element in judge

    int printa = 0, printb = 0, printc = 0;

    // Entry code
    while (1) {
        while (*synchronization != 1)
            ;
        // Critical section

        // Check for termination
        if (*terminate == 1)
            exit(0);

        int indexa, indexb, indexc;

        int curr = *current;

        printf("\nNew number came is %d\n", curr);

        indexa = search(arrA, sizea, curr);
        if (indexa != -1)
            rmv(arrA, indexa);
        if (!iscomplete(arrA, sizea)) {
            display(arrA, sizea, 'A');
        } else if (printa == 0) {
            printf("\nA got access to pizza\n");
            printa++;
        }

        indexb = search(arrB, sizeb, curr);
        if (indexb != -1)
            rmv(arrB, indexb);
        if (!iscomplete(arrB, sizeb)) {
            display(arrB, sizeb, 'B');
        } else if (printb == 0) {
            printf("\nB got access to pizza\n");
            printb++;
        }

        indexc = search(arrC, sizec, curr);
        if (indexc != -1)
            rmv(arrC, indexc);
        if (!iscomplete(arrC, sizec)) {
            display(arrC, sizec, 'C');
        } else if (printc == 0) {
            printf("\nC got access to pizza\n");
            printc++;
        }

        // Exit section
        *synchronization = 0;
        sleep(3);
    }

    return 0;
}

// Function to search for a specific element
int search(int a[], int n, int ele) {
    for (int i = 0; i < n; i++)
        if (a[i] == ele)
            return i;
    return -1;
}

// Function to update values
void rmv(int a[], int index) {
    a[index] = -99;
}

void display(int arr[], int n, char var) {
    printf("\n     ----------------------\n");
    printf("-->|");
    printf("%c", var);
    for (int i = 0; i < n; i++)
        if (arr[i] == -99)
            printf("   |");
        else
            printf(" %d |", arr[i]);
    printf("\n     -----------------------\n");
}

int iscomplete(int a[], int n) {
    for (int i = 0; i < n; i++)git add .

        if (a[i] != -99)
            return 0;
    return 1;
}
