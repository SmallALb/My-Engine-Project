#pragma once

#include "FISH/Core.h"
//Component interface
class API_ Component {
public:
  virtual std::string getComponentName() const = 0;
};

