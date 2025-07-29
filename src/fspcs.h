#pragma once

#include <cstdio>
#include <iostream>
#include<filesystem>
#include <memory>
#include <utility>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <nlohmann/json.hpp>

#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <queue>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <variant>
#include <any>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using string = std::string;

#ifdef _WIN32
    #include <windows.h>
#endif
//屏幕2d坐标转为比例坐标
glm::vec2 ToProportCoord(const glm::vec2& src, float width, float height);

glm::vec2 ToNDC(const glm::vec2& src, float width, float height);




