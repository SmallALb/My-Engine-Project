#pragma once 

#include "FISH/System.h"

namespace FISH {
  class RenderSystem : public FISH_System {
    RenderSystem() {}
  public:
    
  private:
    static RenderSystem* ptr;
  };  
}