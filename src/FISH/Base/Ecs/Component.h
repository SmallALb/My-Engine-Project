#pragma once

#include "FISH/Core.h"
//Component interface
class API_ Component {
public:
  bool alive{0};

  virtual std::string getComponentName() const = 0;
};

