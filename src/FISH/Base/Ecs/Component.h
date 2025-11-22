#pragma once

#include "FISH/Core.h"
//组件接口
class API_ Component {
public:
  virtual std::string getComponentName() const = 0;
};

