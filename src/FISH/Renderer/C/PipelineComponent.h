#pragma once

namespace FISH {

  struct PipelineInfo : public Component {
    PipelineInfo(): Shaders{0, 0, 0, 0, 0, 0}{}

    union{
      struct {
        uint32_t vertexShader;
        uint32_t fragmentShader;
        uint32_t geometryShader;
        uint32_t computeShader;
        uint32_t tessControlShader;
        uint32_t tessValuationShader;
      };
      uint32_t Shaders[6];
    };

    CullMode cullMode;
    FrontFace FontFace;
    PolygonMode polygonMode;
    CompareOp DepthTestCompare;
    BlendOp srcBlenOp;
    BlendOp dstBlenOp;
    StencilOp stencilFunc;
    StencilOp stencilRef;
    StencilOp stencilMask;

    int PipleStatusMask{0};
    
    uint32_t vertexArray{0};
    std::string getComponentName() const override {return "PipelineInfo";}
  };

}