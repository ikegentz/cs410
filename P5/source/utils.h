//
// Created by ikegentz on 10/10/18.
//

#ifndef CS410_UTILS_H
#define CS410_UTILS_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace Utils
{
    static void tokenize_line(std::string line, std::vector<std::string> &tokens)
    {
        std::string buffer;
        std::stringstream ss(line);

        // tokenize the line to grab model data
        while(ss >> buffer)
            tokens.push_back(buffer);
    }
}

#endif //CS410_UTILS_H
