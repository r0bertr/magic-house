#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

typedef std::pair<glm::vec3, glm::vec3> CollisionSize;

class CollisionDetector {
public:
    static CollisionDetector *getInstance();
    static void setValue(const GLchar *name, glm::vec3 anchor, glm::vec3 size);
    static void setValue(std::string name, glm::vec3 anchor, glm::vec3 size);

    bool judge(glm::vec3 pos);

private:
    CollisionDetector();
    ~CollisionDetector();

    std::map<std::string, CollisionSize> *collisionSizes;
};