// **************************************************************
// *
// *  Preproccessing.cpp
// *
// *  Based on LB_Keogh and LSH algorithm, perform fast Dynamic Time Wrapping
// *
// **************************************************************
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
#include <utility>
#include <random>
#include "RTree_HD.h"
#include <queue>
#include <ctime>


using namespace std;


#define M 5000 //The number of time series
#define T 256 //The length of a time serie
#define D 64 //The dimension of a time point
#define bandwidth  0.12*T//Used for Sakoe-Chiba Band restriction, 0<=bandwidth<=T
//#define slope_variance 1 //Used for Itakura parallelogram restriction
#define constraint 4 //Itakura parallelogram window, must be smaller than T
#define PAAReductionFactor 4 //the equal amount for each cell, must be a factor of T
#define L 4//The number of LSH function groups
#define K 5//The number of LSH functions in each group
#define W 1//Numerical constant
#define threshold 120
#define BlockNum T/PAAReductionFactor//The number of blocks left after using PAA
#define Epsilon 0.03*sqrt(D)*sqrt(T)//Threshold used in RTree RangeSearch
#define R 0.02*sqrt(D)
#define KNN 0.05*T

//Computer Euclidean distance between two datasets with length N
float distance(float p[], float q[], int N)
{
	float dist = 0;
	for (int i = 0; i<N; i++)
	{
		dist += (p[i] - q[i])*(p[i] - q[i]);
	}
	return sqrt(dist);
}

//Computer Euclidean distance between two points with dimension d
float distance_HD(float p[], float q[], int d)
{
    float dist=0;
    for(int i=0;i<d;i++){
        dist+=(p[i]-q[i])*(p[i]-q[i]);
    }
    return dist;
}

//Computer Euclidean distance between two series with dimension d
float distance_HD(float** p, float** q)
{
    float dist=0;
    for(int i=0;i<T;i++){
        for(int j=0;j<D;j++){
            dist+=(p[i][j]-q[i][j])*(p[i][j]-q[i][j]);
            }
    }
    return sqrt(dist);
}

//load dataset from files
/*float**  load_data(const char* filename, int row, int col)
{
    ifstream file(filename); // declare file stream:
    string value;
    int i,j;
    int count = -1;
    string num;
    float **data = new float*[row];
    for (int i = 0; i < row; i++)
        data[i] = new float[col];
    
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            getline(file, value, ' ');
            num = string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
        getline(file, value, '\n');
    }
    file.close();
    return data;
}*/
float***  load_data(const char* filename,int m, int timepoint, int dimension)

{
    
    ifstream file(filename); // declare file stream:
    
    string value;
    
    string num;
    
    int i,j,k;
    
    int count = -1;
    
    float ***data = new float**[m];
    
    for (i = 0; i<m; i++) {
        
        data[i] = new float*[timepoint];
        
        for (j = 0; j<timepoint; j++) {
            
            data[i][j] = new float[dimension];
            
        }
        
    }
    
    
    
    
    for ( i = 0; i<m; i++) {
        
        for ( j = 0; j<timepoint; j++) {
            
            for (k = 0; k<(dimension); k++) {
                
                getline(file, value, ' ');
                
                num = string(value, 0, value.length());
                
                
                data[i][j][k] = ::atof(num.c_str());
                
            }
            
            getline(file, value, '\n');
            
        }
        
    }
    
    file.close();
    
    return data;
    
}

//normalize input datasets to the range of [0,1]
void normalization_HD(float***&p){
    float max[D]={-INFINITY};
    float min[D]={INFINITY};
    
    for(int d=0;d<D;d++){
        for(int i=0;i<M;i++){
            for(int j=0;j<T;j++){
                if(p[i][j][d]>=max[d])
                    max[d]=p[i][j][d];
                if(p[i][j][d]<min[d])
                    min[d]=p[i][j][d];
            }
        }
    }
    for(int i=0;i<M;i++){
        for(int j=0;j<T;j++){
            for(int d=0;d<D;d++){
                p[i][j][d]=(p[i][j][d]-min[d])/(max[d]-min[d]);
            }
        }
    }

}


//Basic one dimensional DTW
float DTW_Basic(float* p, float* q)
{
    float gamma[T][T];
    float dist[T][T];
    for(int i=0;i<T;i++){
        for(int j=0;j<T;j++){
            dist[i][j]=sqrt((p[i]-q[j])*(p[i]-q[j]));//distance(p[i],q[j]);
        }
    }
    gamma[0][0]=dist[0][0];
    for(int i=1;i<T;i++){
        gamma[0][i]=dist[0][i]+gamma[0][i-1];
        //gamma[0][i]=INFINITY;
    }
    for(int i=1;i<T;i++){
        gamma[i][0]=dist[i][0]+gamma[i-1][0];
        //gamma[i][0]=INFINITY;
    }
    for(int i=1;i<T;i++){
        for(int j=1;j<T;j++){
            if ((j - i < bandwidth) && (j - i > -bandwidth))//Rectangle restriction
                gamma[i][j]=min(gamma[i-1][j-1],min(gamma[i-1][j],gamma[i][j-1]))+dist[i][j];
            else gamma[i][j]=dist[i][j];
        }
    }
    
    //cout<<gamma[95][95]<<endl;
    vector<pair<int,int> > pair_vector;
    int i=0;
    int j=0;
    
    while(i<T-1&&j<T-1){
        if(i==T-2&&j!=T-2)
            j+=1;
        else if(j==T-2&&i!=T-2)
            i+=1;
        else if(i==T-2&&i==T-2){
            i+=1;
            j+=1;
        }
        else{
            if(gamma[i+1][j+1]-dist[i+1][j+1]==gamma[i+1][j])
                i+=1;
            else if(gamma[i+1][j+1]-dist[i+1][j+1]==gamma[i][j+1])
                j+=1;
            else{
                i+=1;
                j+=1;
            }
        }
        pair_vector.push_back(make_pair(i,j));
    }
    //cout<<"(p, q)"<<endl;
    float cost=0;
    for(int i=0;i<pair_vector.size();i++){
        //cout << "Pair: "<<pair_vector[i].first << ", " << pair_vector[i].second << endl;
        cost=cost+(p[pair_vector[i].first]-q[pair_vector[i].second])*(p[pair_vector[i].first]-q[pair_vector[i].second]);
        //cout<<cost<<endl;
    }
    //cout<<"Cost calculated using pairs: "<<sqrt(cost)<<endl;
    //return sqrt(gamma[T-1][T-1]);
    //cout<<"Cost calculated using gamma: "<<sqrt(gamma[T-1][T-1])<<endl;
    /*cout<<"Cost calculated using gamma: "<<sqrt(gamma[T-1][T-2])<<endl;
     cout<<"Cost calculated using gamma: "<<sqrt(gamma[T-2][T-1])<<endl;
     cout<<"Cost calculated using gamma: "<<sqrt(gamma[T-2][T-2])<<endl;
     cout<<"Cost calculated using gamma: "<<sqrt(gamma[T-1][T-3])<<endl;
     cout<<"Cost calculated using gamma: "<<sqrt(gamma[T-3][T-1])<<endl;*/
    return cost;
}
//Multi-dimension DTW by calculating DTW in every dimension and sum them up, using DTW_Basic function
float DTW_1D(float** p, float** q){
    float gamma[D][D];
    float dist[D][D];
    float** p_new=new float*[D];
    float** q_new=new float*[D];
    for (int i = 0; i < D; i++){
        p_new[i] = new float[T];
        q_new[i] = new float[T];
    }
    for(int i=0;i<D;i++){
        for(int j=0;j<T;j++){
            p_new[i][j]=p[j][i];
            q_new[i][j]=q[j][i];
        }
    }
    float cost=0;
    for(int i=0;i<D;i++){
        cost+=DTW_Basic(p_new[i],q_new[i]);
    }
    
    
    for (int i = 0; i < D; i++){
        delete [] p_new[i];
        delete [] q_new[i];
    }
    delete [] p_new;
    delete [] q_new;
    
    return sqrt(cost);//square root? Already did in DTW_Basic
}


//Basic one dimensional DTW
float DTW_HD(float** p, float** q)
{
    float gamma[T][T];
    float dist[T][T];
    for(int i=0;i<T;i++){
        for(int j=0;j<T;j++){
            dist[i][j]=distance_HD(p[i],q[j],D);
        }
    }
    gamma[0][0]=dist[0][0];
    for(int i=1;i<T;i++){
        gamma[0][i]=dist[0][i]+gamma[0][i-1];
        gamma[i][0]=dist[i][0]+gamma[i-1][0];
        //gamma[i][0]=INFINITY;
    }
    float temp=0;
    for(int i=1;i<T;i++){
        for(int j=1;j<T;j++){
            if ((j - i < bandwidth) && (j - i > -bandwidth))//Rectangle restriction
                gamma[i][j]=min(gamma[i-1][j-1],min(gamma[i-1][j],gamma[i][j-1]))+dist[i][j];
            else gamma[i][j]=dist[i][j];
            if(gamma[i][j]>=temp){
                temp=gamma[i][j];
            }
        }
    }
    //cout<<gamma[95][95]<<endl;
    vector<pair<int,int> > pair_vector;
    int i=0;
    int j=0;
    while(i<T-1&&j<T-1){
        if(i==T-2&&j!=T-2)
            j+=1;
        else if(j==T-2&&i!=T-2)
            i+=1;
        else if(i==T-2&&i==T-2){
            i+=1;
            j+=1;
        }
        else{
            if(gamma[i+1][j+1]-dist[i+1][j+1]==gamma[i+1][j])
                i+=1;
            else if(gamma[i+1][j+1]-dist[i+1][j+1]==gamma[i][j+1])
                j+=1;
            else{
                i+=1;
                j+=1;
            }
        }
        //pair_vector.push_back(make_pair(i,j));
        pair_vector.push_back(make_pair(i,j));
    }
    float cost=0;
    for(int i=0;i<pair_vector.size();i++){
        //cout << "Pair: "<<pair_vector[i].first << ", " << pair_vector[i].second << endl;
        cost=cost+distance_HD(p[pair_vector[i].first],q[pair_vector[i].second],D);
    }
    return sqrt(cost);
}

vector<int> DTW_GroundTruth_Range(float**query, float*** datasets){
    vector<int> candidate;
    for(int i=0;i<M;i++){
        if(DTW_HD(query,datasets[i])<=Epsilon){
            candidate.push_back(i);
        }
    }
    return candidate;
}

vector<int> DTW_GroundTruth_KNN(float**query, float*** datasets){
    vector<pair<int,float> > candidate_KNN;
    int count=0;
    struct sort_pred {
        bool operator()(const std::pair<int,float> &left, const std::pair<int,float> &right) {
            return left.second < right.second;
        }
    };
    for(int m=0;m<M;m++){
        if(count<KNN){
            candidate_KNN.push_back(make_pair(m,DTW_HD(query,datasets[m])));
        }
        else{
            sort(candidate_KNN.begin(),candidate_KNN.end(),sort_pred());
            float temp=DTW_HD(query,datasets[m]);
            if(temp<candidate_KNN.back().second){
                candidate_KNN.pop_back();
                candidate_KNN.push_back(make_pair(m,temp));
            }
        }
        count++;
    }
    vector<int> KNN_output;
    for(vector<pair<int,float> >::iterator it=candidate_KNN.begin();it!=candidate_KNN.end();++it){
        KNN_output.push_back((*it).first);
    }
    return KNN_output;
}

// Compute one dimensional PAA Upper Bound
float** computePAAUpper_HD(float**& p){
    float** temp=new float*[T];
    float** upper=new float*[BlockNum];
    for(int i=0;i<T;i++)
        temp[i]=new float[D];
    for(int i=0;i<BlockNum;i++)
        upper[i]=new float[D];
    
    for(int d=0;d<D;d++){
        for(int i=0;i<T;i++){
            float largest_temp=p[i][d];
                if(i<constraint){
                    for(int j=0;j<i+constraint+1;j++){
                        if(p[j][d]>=largest_temp)
                            largest_temp=p[j][d];
                    }
                }
                else if(i<T-constraint){
                    for(int j=i-constraint;j<i+constraint+1;j++){
                        if(p[j][d]>=largest_temp)
                            largest_temp=p[j][d];
                    }
                }
                else{
                    for(int j=i-constraint;j<T;j++){
                        if(p[j][d]>=largest_temp)
                            largest_temp=p[j][d];
                    }
                }
            temp[i][d]=largest_temp;
        }
    }//build Sakoe-Chiba band envelope*/
    for(int d=0;d<D;d++){
        for(int i=0;i<T;i=i+PAAReductionFactor){
            float largest_temp=temp[i][d];
            for(int j=i;j<i+PAAReductionFactor;j++){
                if(temp[j][d]>=largest_temp)
                    largest_temp=temp[j][d];
            }
            upper[i/PAAReductionFactor][d]=largest_temp;
        }//Apply PAA
    }

    for(int i=0;i<T;i++)
        delete[] temp[i];
    delete[] temp;
    return upper;
}

// Compute one dimensional PAA Lower Bound
float** computePAALower_HD(float**& p){
    float** temp=new float*[T];
    float** lower=new float*[BlockNum];
    for(int i=0;i<T;i++)
        temp[i]=new float[D];
    for(int i=0;i<BlockNum;i++)
        lower[i]=new float[D];
    
    for(int d=0;d<D;d++){
        for(int i=0;i<T;i++){
            float smallest_temp=p[i][d];
            if(i<constraint){
                for(int j=0;j<i+constraint+1;j++){
                    if(p[j][d]<=smallest_temp)
                        smallest_temp=p[j][d];
                }
            }
            else if(i<T-constraint){
                for(int j=i-constraint;j<i+constraint+1;j++){
                    if(p[j][d]<=smallest_temp)
                        smallest_temp=p[j][d];
                }
            }
            else{
                for(int j=i-constraint;j<T;j++){
                    if(p[j][d]<=smallest_temp)
                        smallest_temp=p[j][d];
                }
            }
            temp[i][d]=smallest_temp;
        }
    }//build Sakoe-Chiba band envelope*/
    
    for(int d=0;d<D;d++){
        for(int i=0;i<T;i=i+PAAReductionFactor){
            float smallest_temp=temp[i][d];
            for(int j=i;j<i+PAAReductionFactor;j++){
                if(temp[j][d]<=smallest_temp)
                    smallest_temp=temp[j][d];
            }
            lower[i/PAAReductionFactor][d]=smallest_temp;
        }
    }//Apply PAA
    //for(int i=0;i<T;i++)
        //cout<<lower[i/PAAReductionFactor][0]<<endl;
    
    for(int i=0;i<T;i++)
        delete[] temp[i];
    delete[] temp;
    return lower;
}

/* 
 * Compute LB_PAA distance based on previous computed PAA Upper Bound and Lower Bound
 * Calculated dimension by dimension
 */
float compute_LB_PAA_HD(float** standard, float** q){
    float** upperBound=computePAAUpper_HD(standard);
    float** lowerBound=computePAALower_HD(standard);
    
    float** q_PAA=new float*[BlockNum];
    for(int i=0;i<BlockNum;i++)
        q_PAA[i]=new float[D];
    
    for(int d=0;d<D;d++){
        for(int i=0;i<T;i=i+PAAReductionFactor){
            float sum_temp=0;
            for(int j=i;j<i+PAAReductionFactor;j++){
                sum_temp+=q[j][d];
            }
            q_PAA[i/PAAReductionFactor][d]=sum_temp/PAAReductionFactor;
        }
    }
    
    float LB_PAA=0;
    for(int d=0;d<D;d++){
        for(int i=0;i<BlockNum;i++){
            if(q_PAA[i][d]>upperBound[i][d])
                LB_PAA+=(q_PAA[i][d]-upperBound[i][d])*(q_PAA[i][d]-upperBound[i][d]);
            else if(q_PAA[i][d]<lowerBound[i][d])
                LB_PAA+=(q_PAA[i][d]-lowerBound[i][d])*(q_PAA[i][d]-lowerBound[i][d]);
        }
    }
    for(int i=0;i<BlockNum;i++){
        delete[] upperBound[i];
        delete[] lowerBound[i];
        delete[] q_PAA[i];
    }
    delete[] upperBound;
    delete[] lowerBound;
    delete[] q_PAA;
    //cout<<LB_PAA<<endl;
    return sqrt(PAAReductionFactor*LB_PAA);
}

float MINDIST_HD(float** query_series, RTree::Rect MBR){
    float dist=0;
    float** upperBound=computePAAUpper_HD(query_series);
    float** lowerBound=computePAALower_HD(query_series);
    for(int d=0;d<D;d++){
        for(int i=0;i<BlockNum;i++){
            if(lowerBound[i][d]>MBR.m_max[i][d])
                dist+=(lowerBound[i][d]-MBR.m_max[i][d])*(lowerBound[i][d]-MBR.m_max[i][d]);
            else if(upperBound[i][d]<MBR.m_min[i][d])
                dist+=(upperBound[i][d]-MBR.m_min[i][d])*(upperBound[i][d]-MBR.m_min[i][d]);
        }
    }
    dist=T*dist/BlockNum;
    
    for(int i=0;i<BlockNum;i++){
        delete[] upperBound[i];
        delete[] lowerBound[i];
    }
    delete[] upperBound;
    delete[] lowerBound;
    //cout<<"MINDIST :"<<sqrt(dist)<<endl;
    return sqrt(dist);
}
void RangeSearch(float** series, float bound, RTree::Node* current, vector<int>& candidate, float*** standard,int& count){
    if(!current->IsLeaf()){
        for(int i=0;i<current->m_count;i++){
            if(MINDIST_HD(series, current->m_branch[i].m_rect)<=bound){
                RangeSearch(series, bound, current->m_branch[i].m_child,candidate,standard, count);
            }
        }
    }
    else{
        for(int i=0;i<current->m_count;i++){
            if(compute_LB_PAA_HD(series,standard[current->m_branch[i].m_data])<=bound){
                count++;
                if(DTW_HD(series,standard[current->m_branch[i].m_data])<=bound)//using DTW_HD
                    candidate.push_back(current->m_branch[i].m_data);
            }
        }
    }
}
vector<int> RangeSearch(float** series, float bound, RTree::Node* root, float*** standard, int& search_count){
    vector<int> candidate;
    RangeSearch(series, bound, root, candidate,standard,search_count);
    return candidate;
}
vector<int> DTW_KNNSearch(float **series, float*** datasets, RTree::Node* root, int& search_count){
    struct sort_pred {
        bool operator()(const std::pair<int,float> &left, const std::pair<int,float> &right) {
            return left.second < right.second;
        }
    };
    /*
     *  If MinPriorityQueue.first!=-1, it's a PAA point C(time series), MinPriorityQueue.second.first is NULL and and it's the dataset ID
     *  and MinPriorityQueue.first stands for the series ID;
     *  If MinPriorityQueue.first==-1, it's a internal or leaf node
     */
    
    priority_queue< pair < int, pair<RTree::Node*, float> > > MinPriorityQueue;
    list<pair<int,float> > temp;
    vector<int> result;
    
    MinPriorityQueue.push(make_pair(-1,make_pair(root,0)));
    while(!MinPriorityQueue.empty()){
        //temp.sort(sort_pred());可加可不加
        pair<int,pair<RTree::Node*,float> > top=MinPriorityQueue.top();
        for(list<pair<int,float> >::iterator it=temp.begin();it!=temp.end();it++){
            search_count++;
            //cout<<temp.size()<<": "<<(*it).second<<endl;
            //if(DTW_HD(series,datasets[(*it).first])<=Epsilon){
            if((*it).second<=top.second.second){
                cout<<(*it).second<<" "<<compute_LB_PAA_HD(series,datasets[(*it).first])<<endl;
                result.push_back((*it).first);
                it=temp.erase(it);
            }
            if(result.size()>=KNN){
                return result;
            }
        }
        MinPriorityQueue.pop();
    
        if(top.first!=-1){
            temp.push_back(make_pair(top.first, DTW_HD(series,datasets[top.first]) ) );
        }
        else if(top.second.first->IsInternalNode()){
            for(int i=0;i<top.second.first->m_count;i++){
                MinPriorityQueue.push(make_pair(-1,make_pair(top.second.first->m_branch[i].m_child,MINDIST_HD(series,top.second.first->m_branch[i].m_rect))));
            }
        }
        else if(top.second.first->IsLeaf()){
            for(int i=0;i<top.second.first->m_count;i++){
                MinPriorityQueue.push(make_pair(top.second.first->m_branch[i].m_data, make_pair(top.second.first->m_branch[i].m_child,compute_LB_PAA_HD(series,datasets[top.second.first->m_branch[i].m_data]))));
                //cout<<compute_LB_PAA_HD(series,datasets[top.second.first->m_branch[i].m_data])<<endl;
            }
        }
    }
    return result;
}

    /*The following part is LSH method query*/
float*** generateRandom(){
    default_random_engine generator(time(NULL));
    normal_distribution<float> distribution(0.0,1.0);
    float ***random;
    random = new float **[L];
    for (int l = 0; l<L; l++){
        random[l] = new float *[K];
        for(int k=0;k<K;k++){
            random[l][k] = new float [D];
        }
    }
    for (int l = 0; l < L; l++){
        for (int k = 0; k < K; k++){
            for(int d=0;d<D;d++){
                float temp=distribution(generator);;
                while(temp<0||temp>1)
                    temp=distribution(generator);
                random[l][k][d]=temp;
            }
        }
    }
    return random;
}

//Calculate LSH for the whole datasets
float **** CalculateLSH(float*** p, float*** random){
    float ****hash_value;
    hash_value=new float***[M];
    for(int m=0;m<M;m++){
        hash_value[m]=new float**[L];
        for(int l=0;l<L;l++){
            hash_value[m][l]=new float*[K];
            for(int k=0;k<K;k++){
                hash_value[m][l][k]=new float[T];
            }
        }
    }
    for(int m=0;m<M;m++){
        for(int l=0;l<L;l++){
            for(int k=0;k<K;k++){
                for(int n=0;n<T;n++){
                    float temp=0;
                    for(int d=0;d<D;d++){
                        temp+=p[m][n][d]*random[l][k][d];
                    }
                    hash_value[m][l][k][n]=floor(temp/W);
                }
            }
        }
    }
    return hash_value;
}

//Calculate LSH for one dataset
float *** CalculateLSH(float** p, float*** random){
    float ***hash_value;
    hash_value=new float**[L];
    for(int l=0;l<L;l++){
        hash_value[l]=new float*[K];
        for(int k=0;k<K;k++){
            hash_value[l][k]=new float[T];
        }
    }
    

    for(int l=0;l<L;l++){
        for(int k=0;k<K;k++){
            for(int n=0;n<T;n++){
                float temp=0;
                for(int d=0;d<D;d++){
                    temp+=p[n][d]*random[l][k][d];
                }
                hash_value[l][k][n]=floor(temp/W);
            }
        }
    }
    return hash_value;
}

vector<int> REnvelope_GroundTruth(float** query, float*** datasets){
    vector<int> candidate;
    for(int m=0;m<M;m++){
        bool isTrue=true;
        for(int i=0;i<T;i++){
            for(int d=0;d<D;d++){
                if(sqrt((query[i][d]-datasets[m][i][d])*(query[i][d]-datasets[m][i][d]))>R){
                    isTrue=false;
                    break;
                }
            }
        }
        if(isTrue)
            candidate.push_back(m);
    }
    return candidate;
}

vector<int> LSH_query(float** query, float*** datasets, float*** hash_functions, float**** hash_value){
    float ***query_hash=CalculateLSH(query,hash_functions);//query_hash[L][K][T], hashed values for the query series

    vector<int> candidate;
    for(int m=0;m<M;m++){
        for(int l=0;l<L;l++){
            bool collision=true;
            for(int k=0;k<K;k++){
                int count=0;
                for(int n=0;n<T;n++){
                    if(hash_value[m][l][k][n]==query_hash[l][k][n]){
                        count++;
                    }
                    if(count>=threshold){
                        break;
                    }
                }
                if(count<threshold){
                    collision=false;
                    break;
                }
            }
            if(collision==true){
                candidate.push_back(m);
                break;
            }
        }
    }
    
    for(int l=0;l<L;l++){
        for(int k=0;k<K;k++){
            delete []query_hash[l][k];
        }
        delete []query_hash[l];
    }
    delete []query_hash;
    
    return candidate;
}

vector<int> LSH_KNN(vector<int> candidate, float** query, float*** datasets){
    int count=0;
    struct sort_pred {
        bool operator()(const std::pair<int,float> &left, const std::pair<int,float> &right) {
            return left.second < right.second;
        }
    };
    vector<pair<int,float> > candidate_KNN;
    for(vector<int>::iterator it=candidate.begin();it!=candidate.end();++it){
        if(count<KNN){
            candidate_KNN.push_back(make_pair((*it),DTW_HD(query,datasets[*it])));
            sort(candidate_KNN.begin(),candidate_KNN.end(),sort_pred());
        }
        else{
            sort(candidate_KNN.begin(),candidate_KNN.end(),sort_pred());
            float temp=DTW_HD(query,datasets[*it]);
            if(temp<candidate_KNN.back().second){
                candidate_KNN.pop_back();
                candidate_KNN.push_back(make_pair((*it),temp));
            }
        }
        count++;
    }
    vector<int> KNN_output;
    for(vector<pair<int,float> >::iterator it=candidate_KNN.begin();it!=candidate_KNN.end();++it){
        KNN_output.push_back((*it).first);
        //cout<<(*it).second<<endl;
    }
    
    
    /*vector<pair<int,float> > candidate_KNN;
    for(vector<int>::iterator it=candidate.begin();it!=candidate.end();++it)
        candidate_KNN.push_back(make_pair((*it),DTW_HD(query,datasets[*it])));
    sort(candidate_KNN.begin(),candidate_KNN.end(),sort_pred());
    
    vector<int> KNN_output;
    for(vector<pair<int,float> >::iterator it=candidate_KNN.begin();it!=candidate_KNN.end();++it){
        if(count<KNN){
            KNN_output.push_back((*it).first);
            
        }
        count++;
    }*/
    return KNN_output;

}

vector<int> LSH_LB_Pruning(vector<int> candidate,float*** hash_functions, float**** hash_value, float** query, float*** datasets){
    struct sort_pred {
        bool operator()(const std::pair<int,float> &left, const std::pair<int,float> &right) {
            return left.second < right.second;
        }
    };//from small to large
    float ***query_hash=CalculateLSH(query,hash_functions);
    vector<pair<int,float> > lower_bound;
    for(vector<int>::iterator it=candidate.begin();it!=candidate.end();++it){
        float temp=0;
        for(int l=0;l<L;l++){
            for(int k=0;k<K;k++){
                for(int t=0;t<T;t++){
                    temp+=(query_hash[l][k][t]-hash_value[*it][l][k][t])*(query_hash[l][k][t]-hash_value[*it][l][k][t])*R*R;
                }
            }
        }
        lower_bound.push_back(make_pair((*it),sqrt(temp)));
    }
    sort(lower_bound.begin(),lower_bound.end(),sort_pred());
    
    vector<pair<int,float> > candidate_KNN;
    int count=0;
    int flag=0;
    for(vector<pair<int,float> >::iterator it=lower_bound.begin();it!=lower_bound.end();++it){
        if(count<KNN){
            candidate_KNN.push_back(make_pair((*it).first,DTW_HD(query,datasets[(*it).first])));
        }
        else{
            sort(candidate_KNN.begin(),candidate_KNN.end(),sort_pred());
            float temp=DTW_HD(query,datasets[(*it).first]);
            if(temp<candidate_KNN.back().second){
                candidate_KNN.pop_back();
                candidate_KNN.push_back(make_pair((*it).first,temp));
                flag=0;
            }
            else flag++;
        }
        count++;
        if(flag==10)
            break;
    }
    vector<int> output;
    for(vector<pair<int,float> >::iterator it=candidate_KNN.begin();it!=candidate_KNN.end();++it){
        output.push_back((*it).first);
        //cout<<(*it).second<<endl;
    }
    for(int l=0;l<L;l++){
        for(int k=0;k<K;k++){
            delete []query_hash[l][k];
        }
        delete []query_hash[l];
    }
    delete []query_hash;

    return output;
    
}

vector<int> LSH_Intersection_Pruning(vector<int> candidate,float*** hash_functions, float**** hash_value, float** query, float*** datasets){
    struct sort_LtoS {
        bool operator()(const std::pair<int,float> &left, const std::pair<int,float> &right) {
            return left.second > right.second;
        }
    };//from large to small
    struct sort_StoL {
        bool operator()(const std::pair<int,float> &left, const std::pair<int,float> &right) {
            return left.second < right.second;
        }
    };//from small to large
    float ***query_hash=CalculateLSH(query,hash_functions);

    vector<pair<int,float> > Pro_sim;
    for(vector<int>::iterator it=candidate.begin();it!=candidate.end();++it){
        float lower_temp=0;
        float upper_temp=0;
        for(int l=0;l<L;l++){
            for(int k=0;k<K;k++){
                for(int t=0;t<T;t++){
                    lower_temp+=(query_hash[l][k][t]-hash_value[*it][l][k][t])*(query_hash[l][k][t]-hash_value[*it][l][k][t])*R*R;
                    upper_temp+=(abs(query_hash[l][k][t]-hash_value[*it][l][k][t])+1)*(abs(query_hash[l][k][t]-hash_value[*it][l][k][t])+1)*R*R;
                }
            }
        }
        Pro_sim.push_back(make_pair((*it),(L*K*R*sqrt(T)-lower_temp)/upper_temp));
    }
    sort(Pro_sim.begin(),Pro_sim.end(),sort_LtoS());
    
    vector<pair<int,float> > candidate_KNN;
    int count=0;
    int flag=0;
    for(vector<pair<int,float> >::iterator it=Pro_sim.begin();it!=Pro_sim.end();++it){
        if(count<KNN){
            candidate_KNN.push_back(make_pair((*it).first,DTW_HD(query,datasets[(*it).first])));
        }
        else{
            sort(candidate_KNN.begin(),candidate_KNN.end(),sort_StoL());
            float temp=DTW_HD(query,datasets[(*it).first]);
            if(temp<candidate_KNN.back().second){
                candidate_KNN.pop_back();
                candidate_KNN.push_back(make_pair((*it).first,temp));
                flag=0;
            }
            else flag++;
        }
        count++;
        if(flag==10)
            break;
    }
    vector<int> output;
    for(vector<pair<int,float> >::iterator it=candidate_KNN.begin();it!=candidate_KNN.end();++it){
        output.push_back((*it).first);
        //cout<<(*it).second<<endl;
    }

    for(int l=0;l<L;l++){
        for(int k=0;k<K;k++){
            delete []query_hash[l][k];
        }
        delete []query_hash[l];
    }
    delete []query_hash;
    return output;

}

int main(){
    /*load data*/
	//float*** datasets=new float**[M];
    float*** datasets=load_data("./EEG/datasetfile.txt",M,T,D);
    /*int num=1;
    for(int i=0;i<M/2;i++){
        string count=to_string(num);
        count="./EEG/class_a/"+count+".txt";
        num++;
        datasets[i]=load_data(count.c_str(),T,D);
    }
    num=1;
    for(int i=M/2;i<M;i++){
        string count=to_string(num);
        count="./EEG/class_c/"+count+".txt";
        num++;
        datasets[i]=load_data(count.c_str(),T,D);
    }*/
    /*load data*/
  
    /*normalization*/
    normalization_HD(datasets);
    /*normalization*/
    
    cout<<"LB_PAA Test: "<<compute_LB_PAA_HD(datasets[10],datasets[2])<<endl;
    cout<<"DTW_HD Test: "<<DTW_HD(datasets[10],datasets[2])<<endl;
    cout<<"DTW_1D Test: "<<DTW_1D(datasets[10],datasets[2])<<endl;
    /*cout<<"LB_PAA Test2: "<<compute_LB_PAA_HD(datasets[0],datasets[5000])<<endl;
    cout<<"DTW_HD Test2: "<<DTW_HD(datasets[0],datasets[5000])<<endl;
    cout<<"DTW_1D Test2: "<<DTW_1D(datasets[0],datasets[5000])<<endl;*/
    
    //some testing:
    /*float *temp=new float[T];
    temp[0]=0;
    temp[1]=0;
    for(int i=2;i<T;i++){
        temp[i]=dataset[0][i-1];
        //cout<<temp[i]<<endl;
    }
    for(int i=0;i<T;i++){
    cout<<dataset[2][i]<<endl;
    }
    cout<<"DTW: "<<DTW_Basic(temp,dataset[0])<<endl;
    cout<<"Euclean: "<<distance(temp, dataset[0], T)<<endl;
    delete[] temp;*/
    //cout<<"LB_PAA: "<<compute_LB_PAA_1D(dataset[0],dataset[2])<<endl;
    //cout<<"DTW: "<<DTW_Basic(dataset[0],dataset[2])<<endl;
    //cout<<"Euclean: "<<distance(dataset[0], dataset[2], T)<<endl;
    
    
    
    /*DTW Range Query Ground Truth*/
    /*cout<<"DTW Range Query Ground Truth: "<<endl;
    clock_t beginRQ = clock();
    int count=0;
    vector<int> DTW_groundtruth_Range=DTW_GroundTruth_Range(datasets[500],datasets);
    for(vector<int>::iterator it=DTW_groundtruth_Range.begin();it!=DTW_groundtruth_Range.end();++it){
        cout<<"Candidate series number for DTW range query ground truth: "<<(*it)<<endl;
        count++;
    }
    cout<<"The total number of candidate series for DTW range query ground truth: "<<count<<endl;
    clock_t endRQ = clock();
    double elapsed_secsRQ = double(endRQ - beginRQ) / CLOCKS_PER_SEC;
    cout<<"The time spent for DTW range query ground truth: "<< elapsed_secsRQ<<" seconds."<<endl;*/
    cout<<"/****************************************************************************/"<<endl;
    /*DTW Range Query Ground Truth*/
    
    /*DTW KNN Query Ground Truth*/
    cout<<"DTW KNN Query Ground Truth: "<<endl;
    clock_t beginDTWKNN = clock();
    int countDTWKNN=0;
    vector<int> DTW_groundtruth_KNN=DTW_GroundTruth_KNN(datasets[500],datasets);
    for(vector<int>::iterator it=DTW_groundtruth_KNN.begin();it!=DTW_groundtruth_KNN.end();++it){
        cout<<"Candidate series number for DTW KNN Query ground truth: "<<(*it)<<endl;
        countDTWKNN++;
    }
    cout<<"The total number of candidate series for DTW KNN Query: "<<countDTWKNN<<endl;
    clock_t endDTWKNN = clock();
    double elapsed_secsDTWKNN = double(endDTWKNN - beginDTWKNN) / CLOCKS_PER_SEC;
    cout<<"The time spent for DTW KNN Query  ground truth: "<< elapsed_secsDTWKNN<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
    /*DTW KNN Query Ground Truth*/
    
    /*DTW using LB_PAA RTree Indexing*/
    cout<<"RTree indexing: "<<endl;
    float*** upperMBR=new float **[M];
    float*** lowerMBR=new float **[M];
    for(int i=0;i<M;i++){
        upperMBR[i]=computePAAUpper_HD(datasets[i]);
        lowerMBR[i]=computePAALower_HD(datasets[i]);
    }
    RTree MyTree;
    for(int m=0;m<M;m++){
        MyTree.Insert(upperMBR[m], lowerMBR[m], m);
    }//up to here is building the tree
    
    /*DTW Range Query using LB_PAA RTree Indexing*/
    clock_t begin2=clock();
    int search_times=0;
    vector<int> candidate=RangeSearch(datasets[500],Epsilon,MyTree.m_root,datasets,search_times);
    int count_PAA=0;
    for(vector<int>::iterator it=candidate.begin();it!=candidate.end();++it){
        cout<<"Candidate series number by LB_PAA RTree indexing: "<<(*it)<<endl;
        count_PAA++;
    }
    cout<<"The total number of candidate series for indexing: "<<count_PAA<<endl;
    cout<<"The total number of searchcing time is: "<<search_times<<endl;
    clock_t end2 = clock();
    double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;
    cout<<"The time spent for RTree range searching: "<< elapsed_secs2<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
     /*DTW Range Query using LB_PAA RTree Indexing*/
    
     /*DTW KNN Query using LB_PAA RTree Indexing*/
    clock_t beginDTW_KNN=clock();
    int DTW_KNN=0;
    vector<int> candidateDTW_KNN=DTW_KNNSearch(datasets[500],datasets,MyTree.m_root,DTW_KNN);
    int count_DTW_KNN=0;
    for(vector<int>::iterator it=candidateDTW_KNN.begin();it!=candidateDTW_KNN.end();++it){
        cout<<"Candidate series number by LB_PAA RTree indexing KNN query: "<<(*it)<<endl;
        count_DTW_KNN++;
    }
    cout<<"The total number of candidate series for DTW KNN querying: "<<count_DTW_KNN<<endl;
    cout<<"The total number of searchcing time is: "<<DTW_KNN<<endl;
    clock_t endDTW_KNN = clock();
    double elapsed_secsDTW_KNN = double(endDTW_KNN - beginDTW_KNN) / CLOCKS_PER_SEC;
    cout<<"The time spent for RTree KNN searching: "<< elapsed_secsDTW_KNN<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
     /*DTW KNN Query using LB_PAA RTree Indexing*/
    
    cout<<"LSH method: "<<endl;
    float ***hash_functions=generateRandom();//hash_functions[L][K][D]
    float ****hash_value=CalculateLSH(datasets,hash_functions);//hash_value[M][L][K][T], hashed values for the whole datasets
    
    /*LSH R Envelope Ground Truth*/
    clock_t beginREnvelope = clock();
    vector<int> candidate_LSH_REnvelope=REnvelope_GroundTruth(datasets[500],datasets);
    int LSH_REnvelope_count=0;
    for(vector<int>::iterator it=candidate_LSH_REnvelope.begin();it!=candidate_LSH_REnvelope.end();++it){
        cout<<"Candidate series number for LSH R envelope ground truth: "<<(*it)<<endl;
        LSH_REnvelope_count++;
    }
    cout<<"The total number of candidate series for LSH R envelope ground truth: "<<LSH_REnvelope_count<<endl;
    clock_t endREnvelope = clock();
    double elapsed_secsREnvelope = double(endREnvelope - beginREnvelope) / CLOCKS_PER_SEC;
    cout<<"The time spent for LSH R envelope ground truth: "<< elapsed_secsREnvelope<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
    /*LSH R Envelope Ground Truth*/
    
    
    /*LSH basic*/
    clock_t beginLSH = clock();
    vector<int> candidateLSH=LSH_query(datasets[500],datasets,hash_functions,hash_value);
    int LSH_count=0;
    for(vector<int>::iterator it=candidateLSH.begin();it!=candidateLSH.end();++it){
        cout<<"Candidate series number for LSH querying: "<<(*it)<<endl;
        LSH_count++;
    }
    cout<<"The total number of candidate series for LSH querying: "<<LSH_count<<endl;
    clock_t endLSH = clock();
    double elapsed_secsLSH = double(endLSH- beginLSH) / CLOCKS_PER_SEC;
    cout<<"The time spent for LSH querying: "<< elapsed_secsLSH<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
    /*LSH basic*/
    
    /*LSH KNN without pruning*/
    clock_t beginLSH_KNN = clock();
    vector<int> candidateLSH_KNN=LSH_KNN(candidateLSH, datasets[500], datasets);
    int LSH_KNN_count=0;
    for(vector<int>::iterator it=candidateLSH_KNN.begin();it!=candidateLSH_KNN.end();++it){
        cout<<"Candidate series number for LSH KNN without pruning: "<<(*it)<<endl;
        LSH_KNN_count++;
    }
    cout<<"The total number of candidate series for LSH KNN without pruning: "<<LSH_KNN_count<<endl;
    clock_t endLSH_KNN = clock();
    double elapsed_secsLSH_KNN = double(endLSH_KNN- beginLSH_KNN) / CLOCKS_PER_SEC;
    cout<<"The time spent for LSH KNN without pruning: "<< elapsed_secsLSH_KNN<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
    /*LSH KNN without pruning*/
    
    /*LSH KNN with LB pruning*/
    clock_t beginLSH_KNN_LB = clock();
    vector<int> candidateLSH_KNN_LB=LSH_LB_Pruning(candidateLSH, hash_functions, hash_value,datasets[500], datasets);
    int LSH_KNN_LB_count=0;
    for(vector<int>::iterator it=candidateLSH_KNN_LB.begin();it!=candidateLSH_KNN_LB.end();++it){
        cout<<"Candidate series number for LSH KNN with LB pruning: "<<(*it)<<endl;
        LSH_KNN_LB_count++;
    }
    cout<<"The total number of candidate series for LSH KNN with LB pruning: "<<LSH_KNN_LB_count<<endl;
    clock_t endLSH_KNN_LB = clock();
    double elapsed_secsLSH_KNN_LB = double(endLSH_KNN_LB- beginLSH_KNN_LB) / CLOCKS_PER_SEC;
    cout<<"The time spent for LSH KNN with LB pruning: "<< elapsed_secsLSH_KNN_LB<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
    /*LSH KNN with LB pruning*/
    
    /*LSH KNN with Intersection pruning*/
    clock_t beginLSH_KNN_Intersection = clock();
    vector<int> candidateLSH_KNN_Intersection=LSH_Intersection_Pruning(candidateLSH, hash_functions, hash_value,datasets[500], datasets);
    int LSH_KNN_Intersection_count=0;
    for(vector<int>::iterator it=candidateLSH_KNN_Intersection.begin();it!=candidateLSH_KNN_Intersection.end();++it){
        cout<<"Candidate series number for LSH KNN with Intersection pruning: "<<(*it)<<endl;
        LSH_KNN_Intersection_count++;
    }
    cout<<"The total number of candidate series for LSH KNN with Intersection pruning: "<<LSH_KNN_Intersection_count<<endl;
    clock_t endLSH_KNN_Intersection = clock();
    double elapsed_secsLSH_KNN_Intersection = double(endLSH_KNN_Intersection- beginLSH_KNN_Intersection) / CLOCKS_PER_SEC;
    cout<<"The time spent for LSH KNN with Intersection pruning: "<< elapsed_secsLSH_KNN_Intersection<<" seconds."<<endl;
    cout<<"/****************************************************************************/"<<endl;
    /*LSH KNN with Intersection pruning*/
    
    for (int i = 0; i < M; i++){
        for(int j=0;j<T;j++){
            delete []datasets[i][j];
        }
        delete[] datasets[i];
    }
    delete[] datasets;
    for (int i = 0; i < M; i++){
        for(int j=0;j<BlockNum;j++){
            delete []upperMBR[i][j];
            delete []lowerMBR[i][j];
        }
        delete []upperMBR[i];
        delete []lowerMBR[i];
    }
    delete []upperMBR;
    delete []lowerMBR;
    
    for (int  m= 0; m < M; m++){
        for(int l=0;l<L;l++){
            for(int k=0;k<K;k++){
                delete []hash_value[m][l][k];
            }
            delete []hash_value[m][l];
        }
        delete []hash_value[m];
    }
    delete[] hash_value;
    
    for(int l=0;l<L;l++){
        for(int k=0;k<K;k++){
            delete []hash_functions[l][k];
        }
        delete []hash_functions[l];
    }
    delete []hash_functions;
    

}

