#pragma once

namespace FISH {
  enum RenderFeature : uint32_t {
    NONE = 0,
    DEPTH_TEST = BIT(0),
    DEPTH_WRITE = BIT(1),
    STENCIL_TEST = BIT(2),
    BLEND = BIT(3),
    CULL_FACE = BIT(4),
    SICISSOR_TEST = BIT(5),
    MULTISAMPLE = BIT(6),
    WIREFRAME = BIT(7),
    ALPTHA_TO_COVAGE = BIT(8),
    DEPTH_CLAMP = BIT(9),
    CONSERVATIVE_RASTER = BIT(10),

    QPAQUE = DEPTH_TEST | DEPTH_WRITE | CULL_FACE, 
    TRAN = DEPTH_TEST | BLEND,
    UI = BLEND,
    SHADOW = DEPTH_TEST | DEPTH_WRITE
  };

  enum class CullMode: uint8_t {
    NONE = 0,
    FRONT,
    BACK,
    FONT_AND_BACK
  };

  enum class PolygonMode : uint8_t {
    FILL = 0,
    LINE,
    POINT
  };

  enum class FrontFace : uint8_t {
    CW = 0,
    CCW
  };

  enum class CompareOp : uint32_t {
    NEVER = 0,
    LESS,
    EQUAL,
    LESS_OR_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_OR_EQUAL,
    ALWAYS
  };

  enum class BlendFactor : uint8_t {
     ZERO = 0,
    ONE = 1,
    SRC_COLOR = 2,
    ONE_MINUS_SRC_COLOR = 3,
    DST_COLOR = 4,
    ONE_MINUS_DST_COLOR = 5,
    SRC_ALPHA = 6,
    ONE_MINUS_SRC_ALPHA = 7,
    DST_ALPHA = 8,
    ONE_MINUS_DST_ALPHA = 9,
    CONSTANT_COLOR = 10,
    ONE_MINUS_CONSTANT_COLOR = 11,
    CONSTANT_ALPHA = 12,
    ONE_MINUS_CONSTANT_ALPHA = 13,
    SRC_ALPHA_SATURATE = 14,
    SRC1_COLOR = 15,
    ONE_MINUS_SRC1_COLOR = 16,
    SRC1_ALPHA = 17,
    ONE_MINUS_SRC1_ALPHA = 18,
  };

  enum class BlendOp : uint8_t {
    ADD = 0,
    SUBTRACT = 1,
    REVERSE_SUBTRACT = 2,
    MIN = 3,
    MAX = 4,
    
    // 扩展操作（Vulkan支持）
    ZERO_EXT = 100,
    SRC_EXT = 101,
    DST_EXT = 102,
    SRC_OVER_EXT = 103,
    DST_OVER_EXT = 104,
    SRC_IN_EXT = 105,
    DST_IN_EXT = 106,
    SRC_OUT_EXT = 107,
    DST_OUT_EXT = 108,
    SRC_ATOP_EXT = 109,
    DST_ATOP_EXT = 110,
    XOR_EXT = 111,
    MULTIPLY_EXT = 112,
    SCREEN_EXT = 113,
    OVERLAY_EXT = 114,
    DARKEN_EXT = 115,
    LIGHTEN_EXT = 116,
    COLOR_DODGE_EXT = 117,
    COLOR_BURN_EXT = 118,
    HARD_LIGHT_EXT = 119,
    SOFT_LIGHT_EXT = 120,
    DIFFERENCE_EXT = 121,
    EXCLUSION_EXT = 122,
    INVERT_EXT = 123,
    INVERT_RGB_EXT = 124,
    LINEAR_DODGE_EXT = 125,
    LINEAR_BURN_EXT = 126,
    VIVID_LIGHT_EXT = 127,
    LINEAR_LIGHT_EXT = 128,
    PIN_LIGHT_EXT = 129,
    HARD_MIX_EXT = 130,
    HSL_HUE_EXT = 131,
    HSL_SATURATION_EXT = 132,
    HSL_COLOR_EXT = 133,
    HSL_LUMINOSITY_EXT = 134,
  };

  enum class StencilOp : uint8_t {
    KEEP = 0,
    ZERO = 1,
    REPLACE = 2,
    INCREMENT_AND_CLAMP = 3,
    DECREMENT_AND_CLAMP = 4,
    INVERT = 5,
    INCREMENT_AND_WRAP = 6,
    DECREMENT_AND_WRAP = 7,
  };

  enum class PrimitiType {
    TRIANGLES,
    TRANGLE_STRIP,
    TRAINLE_FAN,
    LINES,
    POINTS
  };

  using PipelineRenderSettings = std::variant<
    CullMode,
    PolygonMode, 
    FrontFace,
    CompareOp,
    BlendFactor,
    BlendOp,
    StencilOp,
    PrimitiType
  >;
  
  struct PipelineHandleI {
    virtual string getGpuHandleAPI() const = 0;
  };


  static const std::array<RenderFeature, 12>& GetMask() {
    static std::array<RenderFeature, 12> Mask{
      NONE,
      DEPTH_TEST,
      DEPTH_WRITE,
      STENCIL_TEST,
      BLEND,
      CULL_FACE,
      SICISSOR_TEST,
      MULTISAMPLE,
      WIREFRAME,
      ALPTHA_TO_COVAGE,
      DEPTH_CLAMP,
      CONSERVATIVE_RASTER,
    };
    return Mask;
  }
}