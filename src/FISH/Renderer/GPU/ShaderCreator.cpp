#include "fspcs.h"
#include "FISH/Base/SparseMap.h"
#include "../D/ShaderData.h"
#include "ShaderCreator.h"
#include "FISH/Renderer/API.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/GLShaderCreator.h"


namespace FISH {
  ShaderGpuHandle ShaderCreator::CreateShader(const string &contex, ShaderType type) {
    std::unique_ptr<ShaderCreator> creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: creator.reset(new GLShaderCreator()); break;
      default:
        FS_INFO("No API!"); return ShaderGpuHandle();
    }
    return creator->create(contex, type);
  }

  void ShaderCreator::Destory(ShaderGpuHandle &handle) {
    std::unique_ptr<ShaderCreator> creator;
    switch(GetAPI()) {
      case RendererAPI::OpenGL: creator.reset(new GLShaderCreator()); break;
      default:
        FS_INFO("No API!"); return;
    }
    creator->destory(handle);
  }
}
