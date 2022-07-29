
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <windows.h>
#include <iostream>


#define PAD  16 //core_i7 cache line = 64 Byte , int_size = 4 Byte => PAD = 64/4= 16


typedef struct {
	int *A, *B, *C;
	int n, m, p;
	int inputThrNum;
		
} DataSet;

int nthreads;

void fillDataSet(DataSet *dataSet);
void printDataSet(DataSet dataSet);
void closeDataSet(DataSet dataSet);
void multiply(DataSet dataSet);
void multiply_Serial(DataSet dataSet);

int main(int argc, char *argv[]){
	DataSet dataSet;

	if (argc < 5){
		printf("[-] Invalid No. of arguments.\n");
		printf("[-] Try -> <n> <m> <p> <thrN>\n");
		printf(">>> ");
		scanf_s("%d %d %d %d", &dataSet.n, &dataSet.m, &dataSet.p, &dataSet.inputThrNum);
	}
	else{
		dataSet.n = atoi(argv[1]);
		dataSet.m = atoi(argv[2]);
		dataSet.p = atoi(argv[3]);
		dataSet.inputThrNum = atoi(argv[4]);
	}
	fillDataSet(&dataSet);

	double start_t, finish_t;


	//printDataSet(dataSet);


	double sum = 0;
	double avg_p = 0;
	double elapsed_time;
	for (int i = 1; i <= 3; i++){
		start_t = omp_get_wtime();
		multiply(dataSet);
		finish_t = omp_get_wtime();
		elapsed_time = finish_t - start_t;
		printf("\nParallel(%dThreads)-Test(%d) : elapsed time = %f",nthreads,i, elapsed_time);
		sum = sum + elapsed_time;
	}
	avg_p = sum/3.0;
	printf("\nParallel(%d Threads) - Average elapsed time = %f",nthreads, avg_p);

	printf("\n\nEnter 'd' to print Data or something else to continue... : ");
	char input;
	std::cin >>input;
	if (input == 'd')
		printDataSet(dataSet);

	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

	double avg_s = 0;
	for (int i = 0; i < 3; i++){
		start_t = omp_get_wtime();
		multiply_Serial(dataSet);
		finish_t = omp_get_wtime();
		elapsed_time = finish_t - start_t;
		printf("\nSerial - Test(%d):elapsed time = %f", i, elapsed_time);
		avg_s += elapsed_time;
	}
	avg_s = avg_s / 3;
	printf("\nSerial - Average elapsed time = %f",avg_s);
	double speedup = avg_s / avg_p;
	printf("\n\nSpeedup = %f",speedup);

	printf("\n\nEnter 'd' to print Data or something else to continue... : ");

	std::cin >> input;
	if (input == 'd')
		printDataSet(dataSet);



	closeDataSet(dataSet);
	system("PAUSE");
	return EXIT_SUCCESS;
}

void fillDataSet(DataSet *dataSet){
	int i, j;

	dataSet->A = (int *)malloc(sizeof(int)* dataSet->n * dataSet->m);
	dataSet->B = (int *)malloc(sizeof(int)* dataSet->m * dataSet->p);
	dataSet->C = (int *)_aligned_malloc(sizeof(int)* dataSet->n * dataSet->p * PAD , 64);

	srand(time(NULL));

	for (i = 0; i < dataSet->n; i++){
		for (j = 0; j < dataSet->m; j++){
			dataSet->A[i*dataSet->m + j] = rand() % 100;
		}
	}

	for (i = 0; i < dataSet->m; i++){
		for (j = 0; j < dataSet->p; j++){
			dataSet->B[i*dataSet->p + j] = rand() % 100;
		}
	}
}

void printDataSet(DataSet dataSet){
	int i, j;

	printf("[-] Matrix A\n");
	for (i = 0; i < dataSet.n; i++){
		for (j = 0; j < dataSet.m; j++){
			printf("%-4d", dataSet.A[i*dataSet.m + j]);
		}
		putchar('\n');
	}

	printf("[-] Matrix B\n");
	for (i = 0; i < dataSet.m; i++){
		for (j = 0; j < dataSet.p; j++){
			printf("%-4d", dataSet.B[i*dataSet.p + j]);
		}
		putchar('\n');
	}

	printf("[-] Matrix C\n");
	for (i = 0; i < dataSet.n; i++){
		for (j = 0; j < dataSet.p; j++){
			printf("%-8d\t", dataSet.C[i*dataSet.p + j*PAD]);
		}
		putchar('\r\n');
	}
}

void closeDataSet(DataSet dataSet){
	free(dataSet.A);
	free(dataSet.B);
	_aligned_free(dataSet.C);
}

void multiply(DataSet dataSet){

	omp_set_num_threads(dataSet.inputThrNum);
	#pragma omp parallel
	{
		int id, nthrds, rowLoBo, rowUpBo;
		int i, j, k, sum;

		id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;

		rowLoBo = id*(dataSet.n / nthrds) ;
		rowUpBo = (id + 1)*(dataSet.n / nthrds);

		for (i = rowLoBo; i < rowUpBo; i++){
			for (j = 0; j < dataSet.p; j++){
				sum = 0;
				for (k = 0; k < dataSet.m; k++){
					sum += dataSet.A[i * dataSet.m + k] * dataSet.B[k * dataSet.p + j];
				}
				dataSet.C[i * dataSet.p + j*PAD] = sum;
			}

		}
	}


}


void multiply_Serial(DataSet dataSet){
	int i, j, k, sum;
	for(i = 0; i < dataSet.n; i++){
		for(j = 0; j < dataSet.p; j++){
			sum = 0;
			for(k = 0; k < dataSet.m; k++){
				sum += dataSet.A[i * dataSet.m + k] * dataSet.B[k * dataSet.p + j];
			}
			dataSet.C[i * dataSet.p + j * PAD] = sum;
		}
	}
}








