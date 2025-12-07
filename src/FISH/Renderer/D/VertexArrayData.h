#pragma once

namespace FISH {
  //### vertexArray interface
  struct VertexArrayHandleI {
    virtual string getGpuHandleAPI() const = 0;
  }; 
}