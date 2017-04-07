/***
Copyright 2017 Yaohai XU

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***/

/**
    FastLSH
    Normalizer.cpp
    Purpose: This file contains functions needed for normalization
    Features: maxim-minim normalization, normalize along th dimensions

    @author Peter Yaohai XU
    @version 1.0 4/07/17
*/

#include "../include/LSH.h"

/**
 *this normalization is do normalize along the column on 2d dataset. the algorithm may have chance to be optimized
 * @param dataset 2d list of double
 * @return normalized 2d list of double
 */
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
