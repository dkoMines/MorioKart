//
// Created by dko on 12/7/2019.
//

#include <glm/glm.hpp>


#ifndef A7_LIGHT_H
#define A7_LIGHT_H


class Light {
public:
    Light(){}

    glm::vec3 location;
    glm::vec3 color;
    // Maybe direction also, not sure what we want for this.
};


#endif //A7_LIGHT_H
