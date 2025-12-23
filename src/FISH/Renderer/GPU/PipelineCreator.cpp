#include "fspcs.h"
#include "../D/PipelineData.h"
#include "PipelineCreator.h"
#include "FISH/Renderer/API.h"
#include "FISH/Log.h"
#include "Platform/OpenGL/GLPipelineCreator.h"

namespace FISH {

  PipelineGpuHandle PipelineCreator::Create(uint32_t Mask, const std::vector<PipelineRenderSettings> &Features) {
    return PipelineGpuHandle();
  }

  void PipelineCreator::Destory(PipelineGpuHandle &handle)
  {
  }

  void PipelineCreator::BuildShaderProgram(PipelineGpuHandle &handle, std::span<uint32_t> view) {

  }

}