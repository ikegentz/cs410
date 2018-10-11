//
// Created by ikegentz on 10/10/18.
//

#include "light_source.h"
#include "utils.h"
#include <vector>
#include <string>

void LightSource::load_light(std::string light_string)
{
    std::vector<std::string> tokens;
    utils::tokenize_line(light_string, tokens);

    position = glm::vec4(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2)),
            std::stof(tokens.at(3)),
            std::stof(tokens.at(4))
    );

    rgb_amount = glm::vec3(
            std::stof(tokens.at(5)),
            std::stof(tokens.at(6)),
            std::stof(tokens.at(7))
            );
}

void LightSource::print()
{
    std::cout << "LIGHT SOURCE\n" <<
    "\t" << "Position: " << glm::to_string(position) << "\n"
    "\t" << "RGB: " << glm::to_string(rgb_amount) <<
    std::endl;
}
