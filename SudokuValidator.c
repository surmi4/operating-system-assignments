#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
* @author : Urmi Sarkar.
* @date : 02-17-2019.
*/
/* You will need a structure to store the information to be passed to each thread (see "Passing Parameters to Each Thread" in the textbook's project description)*/
int **array;
struct data {
    int row;
    int col;
};

/* Declare the thread that checks columns */
void *column_worker(void *param);    // The function parameter is for the structure you defined
void *square_check(void *mn); // Threads for Checking each square
/* You will also need to declare threads that checks rows and 3x3 subgrids */

/* You are suggested to declare an array of integer values that is visible to each thread. The value in the array (0 or 1) indicates whether the worker thread's number is valid (see "Returning Results to the Parent Thread" in the textbook's project description) */
int main(int argc, char *argv[]) {
    /*You need to assign values to the structure variable. Then you can create multiple worker threads by passing the information using the structure variable*/

    /*You need to call pthread_join() for each childer thread so that the parent will wait*/

    /* Finally, after all children returns, you can check the status array that is visible to everyone and see if it is valid. You then print out the final checking result*/
    array = malloc(9 * sizeof(*array)); /* Assuming 9 is the number of rows */
    if (!array) /* If `malloc` failed then error is thrown */
    {
        fputs(stderr, "Error allocating memory; Bailing out!");
        exit(-1);
    }

    int count = 1;
    for (int i = 0; i < 9; i++) {
        array[i] = malloc(9 * sizeof(**array));
        count++;
    }

    /* Code to read data from the file into the array. */
    FILE *fp = fopen(argv[1], "r");

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fscanf(fp, "%d", &array[i][j]);
        }
    }

    /* Print the input array to validate the input is accurate. */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            printf("array[%d][%d]=%d, ", i, j, array[i][j]);
        }
        printf("\n");
    }

    struct data *p;
    p = (struct data *) malloc(sizeof(struct data));
    pthread_t thread[11];
    int i, a;
    void *b;
    int t, l, k;

    /* Creation of Threads for Checking Row and Column */
    for (i = 0; i < 2; i++) {
        t = i;
        a = pthread_create(&thread[i], NULL, column_worker, (void *) t);
        if (a) {
            printf("error");
        }
    }

    /* Creation of Threads for Checking 9 Squares */
    for (l = 0; l <= 6; l = l + 3) {
        for (k = 0; k <= 6; k = k + 3) {
            (*p).row = l;
            (*p).col = k;
            a = pthread_create(&thread[i++], NULL, square_check, (void *) p);
            if (a) {
                printf("error");
            }
        }
    }

    int s = 0;
    for (i = 0; i < 11; i++) {
        pthread_join(thread[i], &b);
        s = s + (int) b;
    }
    /* Finally checking the state of Sudoku. */
    if (s == 11) {
        printf("The Given Sudoku is Valid.\n");
    } else {
        printf("The Given Sudoku is Invalid\n");
    }
    pthread_exit(NULL);
    return 0;
}


/* Thread code for child checking all columns. */
void *column_worker(void *params) {
    int n, m, h = 0;
    m = (int) params;
    int k, i, j;

    for (i = 0; i < 9; i++) {
        k = 1;
        while (k < 10) {
            for (j = 0; j < 9; j++) {
                if (m == 0 && array[i][j] == k) {
                    h++;
                    goto aam;
                } else if (m == 1 && array[j][i] == k) {
                    h++;
                    goto aam;
                }
            }
            aam:
            k++;
        }
    }
    if (h == 81) {
        n = 1;
    } else {
        n = -1;
    }
    pthread_exit((void *) n);
}

/* also need to define threads for checking rows and 3x3 subgrids */
/* Threads for Checking each 3x3 square. */
void *square_check(void *mn) {
    struct data *my_data = (struct data *) mn;
    int i = (*my_data).row;
    int j = (*my_data).col;
    int n, m, p;
    n = i + 3;
    m = j + 3;
    int k, h = 0;
    for (k = 1; k < 10; k++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < m; j++) {
                if (array[i][j] == k) {
                    h++;
                    i = n;
                    j = m;
                }
            }
        }

    }
    if (h == 9) {
        p = 1;
    } else {
        p = -1;
    }
    pthread_exit((void *) p);
}