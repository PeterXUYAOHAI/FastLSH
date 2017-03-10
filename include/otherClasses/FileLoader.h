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

class FileLoader {
public:
    vector2D loadFile(char* filePath, size_t row, size_t col);
protected:
    std::stringstream ss;
private:
    virtual void loadToSS(char* filePath)=0;
    vector2D ssToVector(size_t row, size_t col);

};

class LfsLoader: public FileLoader {
private:
    virtual void loadToSS(char* filePath);
};


class HdfsLoader: public FileLoader {

private:
    size_t fileSize;
    virtual void loadToSS(char* filePath);

};


#endif //FASTLSH_FILELOADER_H
