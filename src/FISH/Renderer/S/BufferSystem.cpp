#include "fspcs.h"
#include "FISH/System.h"
#include "../D/BufferData.h"
#include "../C/BufferComponent.h"
#include "FISH/Log.h"
#include "BufferSystem.h"
#include "FISH/Renderer/GPU/BufferCreator.h"


namespace FISH {
  BufferSystem* BufferSystem::ptr = nullptr;

  void BufferSystem::OnDetach() {
    const auto& entitys = mRegistry.getCreatedEntitys();
    for (auto& [entity, state] : entitys) if (state.alive) {
      destoryEntity(entity);
    }
  }

  uint32_t BufferSystem::createBuffer(const BufferData &vertices) {
    uint32_t entity = mRegistry.create([](uint32_t entity, Registry& Reger) {
      if (!Reger.has_entity(entity)) return; 
      BufferCreator::Destory(Reger.get<BufferGpuHandle>(entity));
      FS_CORE_INFO("sueccesed to destory Buffer entity: {}", entity);
    });

    mRegistry.add<BufferTypeC>(entity, vertices.type);

    if (vertices.type == BufferType::VERTEX){       
      mRegistry.add<BufferLayoutC>(entity);
      auto& Layout = mRegistry.get<BufferLayoutC>(entity);
      Layout.TypeLayout = std::get<0>(vertices.data).second;
    }

    mRegistry.add<BufferGpuHandle>(entity, BufferCreator::CreateBuffer(vertices));     
  
    FS_CORE_INFO("Success to build buffer entity: {}", entity);
    return entity;
  }

  void BufferSystem::destoryEntity(uint32_t entity) {    
    mRegistry.destory(entity);
    FS_CORE_INFO("buffer entity {} destory", entity);
  }

  bool BufferSystem::hasBufferEntity(uint32_t entity) {
    return mRegistry.has_entity(entity);
  }

  BufferHandleI *BufferSystem::getBufferGpuHandle(uint32_t entity) {
    if (!mRegistry.has<BufferGpuHandle>(entity)) return nullptr;
    return mRegistry.get<BufferGpuHandle>(entity).HANDLE;
  }

  BufferType BufferSystem::getBufferType(uint32_t entity) const {
    return mRegistry.get<BufferTypeC>(entity).Type;
  }

  const std::vector<std::pair<InputDataType, bool>>& BufferSystem::getBufferLayout(uint32_t entity) const {
    return mRegistry.get<BufferLayoutC>(entity).TypeLayout;
  }

  BufferSystem *BufferSystem::GetInstance() {
    if (!ptr) ptr = new BufferSystem();
    return ptr;
  }

}