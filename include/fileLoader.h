//
// Created by peter on 17-3-8.
//

#ifndef FASTLSH_FILELOADER_H
#define FASTLSH_FILELOADER_H


#include <vector>
#include <cstdio>
#include <sstream>

typedef std::vector<std::vector<std::vector < double>>> vector3D;
typedef std::vector<std::vector<double>> vector2D;
typedef std::vector<double> vector1D;

class fileLoader {
public:
    fileLoader(char* filePath, size_t row, size_t col):filePath(filePath),row(row),col(col){}
    vector2D loadFile();
protected:
    char* filePath;
    size_t row;
    size_t col;
    std::stringstream ss;
private:
    virtual void loadToSS()=0;
    vector2D ssToVector();

};


#endif //FASTLSH_FILELOADER_H
