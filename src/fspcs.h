#pragma once

#include <cstdio>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <mutex>
#include <condition_variable>


#include <string>
#include <sstream>
#include <functional>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/string_cast.hpp>

using string = std::string;

#ifdef _WIN32
    #include <windows.h>
#endif