#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdlib>
#include <pthread.h>

using namespace std;

//#define NUM_THREADS 40
#define NQ 100
#define NP 10000
#define D   200
#define Threshold   98
#define ThresholdQP  98
#define QS 10000


int NUM_THREADS; 
//load dataset from files


int**  load_data(const char* filename, int row, int col)
{
	ifstream file;// declare file stream:
	file.open(filename);
	string value;
	string num;
	int i, j;
	int count = -1;
	int **data = new int*[row];
	for (int i = 0; i < row; i++)
		data[i] = new int[col];
	
	for (int i = 0; i<row; i++) {
		for (int j = 0; j<col; j++) {
			getline(file, value, ',');
			num = string(value, 0, value.length());
			data[i][j] = ::atof(num.c_str());
		}
		getline(file, value, '\n');
	}
	file.close();
	return data;
}


void writeToCSV(short **matrix, string path, int w,int  l){
	   ofstream file;
           file.open(path.c_str());
       for (int i=0; i< l; i++){
        for(int j=0; j<w;j++){
                file<<matrix[i][j];
                if (j==(w-1))
                        break;
                file<<",";
}
file<<endl;
}
file.close();
}





struct SHitMatrix{
	int matrix_id;
	short **hitmatrix; 
};

struct splithashP{
	int hashP_id;
	int **hashP_part;
	splithashP& operator=(const splithashP& a){
		hashP_id = a.hashP_id;
		hashP_part = a.hashP_part;
		return *this;
	}
};

struct thread_data{
	int **hash_matrix1;
	splithashP SplitHashP;
	SHitMatrix HitMatrix;
	int row1;
	int row2;
	int tablemark;
};

short** preallocate(int m,int n){
	short **hitmatrix;
	hitmatrix = new short *[m];
	for( int i=0; i<m; i++ )
	{
		hitmatrix[i] = new short [n];
	}
	return hitmatrix;
}


// Computer hit matrix for datasetQ and datasetP
void *ComputeHitmatrix(void *threadarg)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int QSize = my_data->row1;
	int PSize = my_data->row2;
	int **HashMatrixQ = my_data->hash_matrix1;
	splithashP a = my_data->SplitHashP;
	int **PPart = a.hashP_part;
	int i, j, hashQ_id, hashP_id;
	for (i = 0; i < QSize; i++)
	{
		for (j = 0; j < PSize; j++)
		{
			for (hashP_id = 0; hashP_id < D/NUM_THREADS; hashP_id++){
				hashQ_id = my_data->tablemark*D/NUM_THREADS + hashP_id;
				if (HashMatrixQ[i][hashQ_id] == my_data->SplitHashP.hashP_part[hashP_id][j])
				my_data->HitMatrix.hitmatrix[i][j] += 1;
			}
		}
	}
	pthread_exit(NULL);
}


int main(int argc, char* argv[])
{  //load datasetQ and datasetP
	int **hashQ, **hashP;
	clock_t start, finish;
	double duration;
	start= clock();
	hashQ = load_data("hashQ.csv", NQ, D);
	hashP = load_data("hashP.csv", D, NP);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "time for load datasets is :" << duration << endl;
	cout<<"please input number of thread"<<endl;
        cin>> NUM_THREADS; 
// Computer hit_matrix
	start = clock();
	//hitQP = ComputeHitmatrix(hashQ, hashP, NQ, NP);
	struct SHitMatrix hm[NUM_THREADS];
	for (int a = 0; a < NUM_THREADS; a++){
		hm[a].matrix_id = a;
		hm[a].hitmatrix = preallocate(NQ, NP);
	}
	
	struct splithashP sp[NUM_THREADS];
	for (int b = 0; b < NUM_THREADS; b++){
		sp[b].hashP_id = b;
		int HashPerThread = D/NUM_THREADS;
		short **hitmatrix;
		sp[b].hashP_part = new int *[HashPerThread];
		for( int i=0; i<HashPerThread; i++ )
		{
			sp[b].hashP_part[i] = new int [NP];
		}
		int **data = new int*[HashPerThread];
		for (int c = 0; c < HashPerThread; c++)
		{
			data[b] = hashP[c+b*HashPerThread];
			for (int d = 0; d < NP; d++){
				sp[b].hashP_part[c][d] = data[b][d];		
			}
			//cout<<data[b][0]<<endl;
		}
	
		//cout << sp[b].hashP_part[1][0] << endl;
	}
	pthread_t threads[NUM_THREADS];
	struct thread_data td[NUM_THREADS];
	int rc;
	int i;
	
	for( i=0; i < NUM_THREADS; i++ ){
		cout <<"main() : creating thread, " << i << endl;
		td[i].hash_matrix1 = hashQ;
		td[i].SplitHashP = sp[i];
		td[i].HitMatrix = hm[i];
		td[i].row1 = NQ;
		td[i].row2 = NP;
		td[i].tablemark = i;
		rc = pthread_create(&threads[i], NULL,
							ComputeHitmatrix, (void *)&td[i]);
		if (rc){
			cout << "Error:unable to create thread," << rc << endl;
			exit(-1);
		}
	}
	//pthread_exit(NULL);
	
	short **Fhit_matrix = new short*[NQ];
	for (int i = 0; i < NQ; i++)
		Fhit_matrix[i] = new short[NP];
	for (int i = 0; i < NQ; i++)
		for (int j = 0; j < NP; j++)
			Fhit_matrix[i][j] = 0;
			
	for (int i = 0; i < NUM_THREADS; i++){
		for (int j = 0; j < NQ ; j++){
			for (int k = 0; k < NP ; k++){
				Fhit_matrix[j][k] += hm[i].hitmatrix[j][k];
			}
		}
	}
	
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "time to Computer hit matrix is :" << duration << endl;
	cout << Fhit_matrix[0][0] << endl;
	
        writeToCSV(Fhit_matrix, "coutput.csv", NP,NQ);
	return 0;
}
