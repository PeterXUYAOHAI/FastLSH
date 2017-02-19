//
// Created by peter on 17-2-19.
//

//maxim minim normalization
//normalize along th dimensions

#include "../include/LSH.h"

//this normalization is do normalize along the column on 2d dataset. the algorithm may have chance to be optimized
vector2D LSH::normalize(vector2D dataset){
    size_t row = dataset.size();
    size_t col = dataset[0].size();

    vector1D maximums(col, -999999);
    vector1D minimums(col, 9999999);

    //find maximums and minimums of each column
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if(dataset[i][j]>= maximums[j])
                maximums[j]=dataset[i][j];
            if(dataset[i][j]<=minimums[j])
                minimums[j]=dataset[i][j];
        }
    }

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            //if max equals min, the value is set to 0.5
            if(maximums[j]==minimums[j])
                dataset[i][j] = 0.5;
            else
                //normalization formula -- (x-min(x))/(max(x)-min(x))
                dataset[i][j] = (dataset[i][j] - minimums[j])/(maximums[j]-minimums[j]);
        }
    }
        return dataset;
}
