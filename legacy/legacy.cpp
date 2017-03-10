//
// Created by peter on 17-3-8.
//

vector2D loadDataFromHDFS(char *filePath, int row, int col, int fileSize);

vector2D loadDataFromLinuxSystem(char *filePath, size_t row, size_t col);

void LSH::loadSetN(char* filePath, int fileSize) {
    NfileSize = fileSize;
    if(!useHdfs) {
        setN = loadDataFromLinuxSystem(filePath, N, D);
    }
    else {
        setN = loadDataFromHDFS(filePath, N, D, NfileSize);
    }
}

void LSH::loadSetQ(char* filePath, int fileSize){
    QfileSize = fileSize;
    if(!useHdfs)
        setQ = loadDataFromLinuxSystem(filePath, Q, D);
    else
        setQ = loadDataFromHDFS(filePath, Q, D, QfileSize);

}

vector2D LSH::loadDataFromLinuxSystem(char* filePath, size_t row, size_t col) {
    std::ifstream file;// declare file stream:
    file.open(filePath);
    vector2D data;
    for (int i = 0; i < row; i++) {
        std::vector<double> temp(col, 0);
        data.push_back(temp);
    }
    std::string value;
    std::string num;
    for (int i = 0; i<row; i++) {
        for (int j = 0; j<col; j++) {
            if (j!=col-1)
                getline(file, value, ',');
            else
                getline(file,value,'\n');
            num = std::string(value, 0, value.length());
            data[i][j] = ::atof(num.c_str());
        }
    }

    file.close();
    return data;
}


bool LSH::setThreadMode(bool useMultiThread) {
    this->useMultiThread = useMultiThread;
    multiThreadMode = 0; //set as default mode - openMP
    return this->useMultiThread;
}