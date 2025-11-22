#pragma once

namespace FISH {
  enum class InputDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
  };

  enum class BufferType {
    VERTEX,
    INDEX,
  };
}