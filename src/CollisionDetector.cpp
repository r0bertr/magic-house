#include "CollisionDetector.hpp"

CollisionDetector *CollisionDetector::getInstance() {

    static CollisionDetector *instance = NULL;
    if (instance == NULL) {
        instance = new CollisionDetector();
    }
    return instance;

}

CollisionDetector::CollisionDetector() {

    collisionSizes = new std::map<std::string, CollisionSize>;

}

CollisionDetector::~CollisionDetector() {

    delete collisionSizes;

}

void CollisionDetector::setValue(const GLchar *name,
    glm::vec3 anchor, glm::vec3 size) {

    CollisionDetector::getInstance()->
        collisionSizes->operator[](std::string(name)) = 
        std::pair<glm::vec3, glm::vec3>(anchor, size);

}

void CollisionDetector::setValue(std::string name,
    glm::vec3 anchor, glm::vec3 size) {

    CollisionDetector::setValue(name.c_str(), anchor, size);
        
}

bool CollisionDetector::judge(glm::vec3 pos) {

    for (std::map<std::string, CollisionSize>::iterator 
        it = collisionSizes->begin();
        it != collisionSizes->end();
        it++) {

        glm::vec3 anchor = it->second.first;
        glm::vec3 size = it->second.second;
        if (anchor.x - 0.5 * size.x <= pos.x &&
            pos.x <= anchor.x + 0.5 * size.x &&
            anchor.y - 0.5 * size.y <= pos.y &&
            pos.y <= anchor.y + 0.5 * size.y &&
            anchor.z - 0.5 * size.z <= pos.z &&
            pos.z <= anchor.z + 0.5 * size.z)
            return true;

    }
    return false;

}
