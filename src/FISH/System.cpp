#include "fspcs.h"
#include "Log.h"
#include "System.h"

namespace FISH {
  //临时写为20
  TaskPool FISH_System::mTaskPool(20);

  FISH_System::FISH_System() {
    
  }

  FISH_System::~FISH_System() {
    Layer::~Layer();
  }

  void FISH_System::submit() {
    mTaskPool.submit([this]() {AsyncUpdate();});
  }
}