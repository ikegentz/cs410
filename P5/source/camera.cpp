//
// Created by ikegentz on 10/10/18.
//

#include "camera.h"
#include "utils.h"
#include <vector>
#include <string>
#include <iostream>

void Camera::load_eye(std::string eye_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(eye_string, tokens);

    eye = glm::vec3(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2)),
            std::stof(tokens.at(3))
            );
}

void Camera::load_look(std::string look_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(look_string, tokens);

    look = glm::vec3(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2)),
            std::stof(tokens.at(3))
    );
}

void Camera::load_up(std::string up_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(up_string, tokens);

    up = glm::vec3(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2)),
            std::stof(tokens.at(3))
    );
}

void Camera::load_d(std::string d_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(d_string, tokens);

    d = std::stof(tokens.at(1));
}

void Camera::load_bounds(std::string bounds_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(bounds_string, tokens);

    bounds = glm::vec4(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2)),
            std::stof(tokens.at(3)),
            std::stof(tokens.at(4))
    );
}

void Camera::load_res(std::string res_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(res_string, tokens);

    res = glm::vec2(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2))
    );
}

void Camera::load_ambient(std::string ambient_string)
{
    std::vector<std::string> tokens;
    Utils::tokenize_line(ambient_string, tokens);

    ambient = glm::vec3(
            std::stof(tokens.at(1)),
            std::stof(tokens.at(2)),
            std::stof(tokens.at(3))
    );
}

void Camera::print()
{
    std::cout << "CAMERA\n" <<
    "\t" << "Eye: " << glm::to_string(eye) << "\n" <<
    "\t" << "Look: " << glm::to_string(look) << "\n" <<
    "\t" << "Up: " << glm::to_string(up) << "\n" <<
    "\t" << "D: " << d << "\n" <<
    "\t" << "Bounds: " << glm::to_string(bounds) << "\n" <<
    "\t" << "Res: " << glm::to_string(res) << "\n" <<
    "\t" << "Ambient: " << glm::to_string(ambient) <<
    std::endl;
}

