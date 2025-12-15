#pragma once

namespace FISH {
  //### Type of Shader
  enum class ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY,
    COMPUTE,
    TESSCONTROL,
    TESSEVALUATION
  };

  //### Type of Shader Data 
  enum class shaderDataType {
    FLOAT, INT, UINT, BOOL,
    VEC2, VEC3, VEC4, IVEC2, IVEC3, IVEC4, UVEC2, UVEC3, UVEC4, BVEC2, BVEC3, BVEC4,
    MAT3, MAT4,
    SAMPLE1D, SAMPLE2D, SAMPLE3D, SAMPLECUBE,
    SAMPLE1DARRAY, SAMPLE2DARRAY, SAMPLECUBEARRAY,
    STRUCT
  };

  //### Type of Shader Attribute
  enum class ShaderDataTypeAttribute {
    NONE = 0,
    INTYP, OUTTYP, INOUT,
    LAYOUT,
    UNIFORM,
    ATTRIBUTE,
    VARYING,
    CONSTTYP,
    SET
  };  


  struct ShaderStructDescribe;
  //struct Member
  struct ShaderStructMember {
    shaderDataType type;
    std::string name;
    size_t arraySize{0};
    size_t struct_index;
    // the struct embeds the other struct
    string structName;
  };

  //struct Descrition of Shader Data
  struct ShaderStructDescribe {
    std::vector<ShaderStructMember> structInner;
  };

  //### Descrition of the Shader Data
  struct ShaderDataDescribe {
    size_t data_index;
    sparse_map<uint8_t, long long> attribute;
    shaderDataType type;
    std::string name;
    //if the Data was Struct, the struct Name will not empty;
    string structName;
    size_t size{0};
  };

  struct LayoutDescribe {
    long long binding{-1};
    long long location{-1};
    size_t data_index;
  };

  struct UniformHandleI {
    virtual string getGpuHandleAPI() const = 0;
  };

  struct ShaderHandleI {
    virtual string getGpuHandleAPI() const = 0;
  };

  struct UniformDescribe {
    bool HasLayout{0};
    size_t data_index;

  };
}