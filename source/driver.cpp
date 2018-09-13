//
// Created by ikegentz on 9/13/18.
//

#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>

#include "driver.h"

Driver::Driver(char *driver_filename)
{
    std::ifstream infile(driver_filename);
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.at(0) == '#')
            continue;

        std::string buffer;
        std::stringstream ss(line);

        std::vector<std::string> tokens;
        while(ss >> buffer)
            tokens.push_back(buffer);

        for(int i = 0; i < tokens.size(); ++i)
        {
            std::cout << tokens.at(i) << std::endl;
        }
    }
}