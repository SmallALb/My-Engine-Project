#include "fspcs.h"

glm::vec2 ToNDC(const glm::vec2& src, float width, float height) {
    float y = height - src.y;

    float ndcx  = src.x / width * 2.0 - 1.0f;
    float ndcy  = y / height * 2.0 - 1.0f;
    
    return glm::vec2(ndcx, ndcy);
}


glm::vec2 ToProportCoord(const glm::vec2& src, float width, float height) {
    glm::vec2 res = {src.x / (width/2), src.y / (height / 2)};
    return res;
}
