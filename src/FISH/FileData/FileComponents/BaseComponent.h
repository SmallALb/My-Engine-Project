#pragma once

#include "../FileData.h"
#include "FISH/Base/Ecs/Component.h"

namespace FISH {
  class FISHFileListener;

  struct FileBaseData : public Component {
    string filePath;
    string fileName;
    string fileText;
    FileType type{FileType::TEXT};
    size_t size{0};
    long watchId{-1};
    std::string getComponentName() const {return "FileBaseData";}
  };
  

}