#include "fspcs.h"
#include "BufferSystem.h"
#include "FISH/Renderer/GPU/BufferCreator.h"


namespace FISH {
  uint32_t GetTypeDataTypeSize(InputDataType typ) {
    switch (typ) {
      case InputDataType::Float:     return 4;
      case InputDataType::Float2:    return 4 * 2;
      case InputDataType::Float3:    return 4 * 3;
      case InputDataType::Float4:    return 4 * 4;
      case InputDataType::Mat3:      return 4 * 3 * 3;
      case InputDataType::Mat4:      return 4 * 4 * 4;
      case InputDataType::Int:       return 4;
      case InputDataType::Int2:      return 4 * 2;
      case InputDataType::Int3:      return 4 * 3;
      case InputDataType::Int4:      return 4 * 4;
      case InputDataType::Bool:      return 1;
      case InputDataType::None:      return 0;
      default:
          break;
    }
    return 0;
  }


  uint32_t BufferSystem::createBuffer(const std::vector<BufferData> &vertices) {
    uint32_t entity = mRegistry.create();
    mRegistry.add<BufferSize>(entity, 0);
    for (size_t i = 0; i<vertices.size(); i++) {
      switch(vertices[i].type) {
        case BufferType::VERTEX: {
          buildVertexBuffer(entity, vertices[i], i);
          break;
        }
        case BufferType::INDEX: {
          break;
        }
      }
    }
    // Erases the BufferSize after build;
    mRegistry.erase<BufferSize>(entity);
    
    return entity;
  }
  
  void BufferSystem::destoryBuffer(uint32_t entity, size_t id) {
    if (!mRegistry.has_ID<BufferGpuHandle>(entity, id)) return;
    auto& handle = mRegistry.get<BufferGpuHandle>(entity, id);
    BufferCreator::Destory(handle);
    mRegistry.erase<BufferGpuHandle>(entity, id);
    mRegistry.erase<VertexBuffer>(entity, id);

    if (!mRegistry.has<BufferGpuHandle>(entity)) {
      mRegistry.destory(entity);
    }

  }

  void BufferSystem::destoryEntity(uint32_t entity) {
    
  }

  void BufferSystem::buildVertexBuffer(uint32_t entity, const BufferData& vertices, size_t id) {
    //Add VertexBuffer
    mRegistry.add<VertexBuffer>(entity, id);
    auto& buffer = mRegistry.get<VertexBuffer>(entity, id);
    auto& Size = mRegistry.get<BufferSize>(entity, 0);
    uint32_t size = GetTypeDataTypeSize(std::get<0>(vertices.data).second);
    //Caculates this entity stride;
    Size.stride += size;
    //Caculates this buffer size
    buffer.offset = Size.stride; 
    mRegistry.add<BufferGpuHandle>(entity, id, BufferCreator::CreateBuffer(vertices));
  }

  void BufferSystem::buildIndexBuffer(uint32_t entity, const BufferData& vertices, size_t id) {
    mRegistry.add<IndexBuffer>(entity, id);
    auto& buffer = mRegistry.get<IndexBuffer>(entity, id);
    mRegistry.add<BufferGpuHandle>(entity, id, BufferCreator::CreateBuffer(vertices));     
  }
}