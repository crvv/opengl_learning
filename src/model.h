#ifndef SIMPLESCENEGRAPH_MODEL_H_
#define SIMPLESCENEGRAPH_MODEL_H_

#include <vector>
#include <fstream>

class ModelUtils {
public:

    template<typename T>
    static void readFileToValue(std::ifstream &file, T &value) {
        file.read(reinterpret_cast<char *>(&value), sizeof(value));
    }

    template<typename T>
    static void readFileToValue(std::ifstream &file, std::vector<T> &list) {
        uint32_t size = 0;
        file.read(reinterpret_cast<char *>(&size), sizeof(size));
        if (size == 0) {
            return;
        }
        list.resize(size);
        file.read(reinterpret_cast<char *>(&list[0]), size * sizeof(T));
    }

    template<typename T, typename... Args>
    static void readFileToValue(std::ifstream &file, T &value, Args &... rest) {
        readFileToValue(file, value);
        readFileToValue(file, rest...);
    }

};


#endif
