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

  
  //### BufferData
  //- Vertex and index;
  struct BufferData {
    //Data
    using Data = std::variant<
      std::pair<
          std::vector<float>,
          InputDataType
        >,
      std::vector<uint32_t>
    >;

    BufferType type;
    Data data;
  };


}