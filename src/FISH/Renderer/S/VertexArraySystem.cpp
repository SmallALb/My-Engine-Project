#include "fspcs.h"
#include "FISH/System.h"
#include "../D/VertexArrayData.h"
#include "../C/VertexArrayComponent.h"
#include "VertexArraySystem.h"
#include "FISH/Log.h"
#include "../GPU/VertexArrayCreator.h"

namespace FISH {
  VertexArraySystem* VertexArraySystem::ptr{nullptr};

  VertexArraySystem *VertexArraySystem::getInstance() {
    if (!ptr) ptr = new VertexArraySystem();
    return ptr;
  }

  void VertexArraySystem::bind(uint32_t entity) {
    auto& handle = mRegistry.get<VertexArrayGpuHandle>(entity);
    VertexArrayCreator::Bind(handle);
  }

  void VertexArraySystem::addBuffer(uint32_t Ventity, uint32_t Bentity) {
    auto& basedata = mRegistry.get<VertexArrayC>(Ventity);
    basedata.bufferEntitys.push_back(Bentity);
    FS_INFO("Add For: {}", Ventity);
    VertexArrayCreator::AddBuffer(mRegistry.get<VertexArrayGpuHandle>(Ventity), Bentity);
  }

  void VertexArraySystem::addBuffer(uint32_t Ventity, const std::initializer_list<uint32_t> &Bentitys) {
    for (auto entity : Bentitys) 
      addBuffer(Ventity, entity);
  }

  void VertexArraySystem::setIndex(uint32_t Ventity, uint32_t Bentity) {
    VertexArrayCreator::SetIndex(mRegistry.get<VertexArrayGpuHandle>(Ventity), Bentity);
  }

  void VertexArraySystem::destoryVertexArray(uint32_t entity) {
    mRegistry.destory(entity);
  }

  void VertexArraySystem::OnDetach() {
    for (auto [e, state] : mRegistry.getCreatedEntitys()) if (state.alive) 
      destoryVertexArray(e); 
  }

  uint32_t VertexArraySystem::create() {
    auto entity = mRegistry.create([](uint32_t entity, Registry& Reger) {
      if (Reger.has<VertexArrayGpuHandle>(entity))  
        VertexArrayCreator::Destory(Reger.get<VertexArrayGpuHandle>(entity));
        FS_CORE_INFO("sueccesed to destory VertexArray entity: {}", entity);
    });

    mRegistry.add<VertexArrayGpuHandle>(entity, std::move(VertexArrayCreator::CreateVertexArray()));
    mRegistry.add<VertexArrayC>(entity);

    auto& handle =  mRegistry.get<VertexArrayGpuHandle>(entity);
    FS_CORE_INFO("The Created HANDLE: {}", (void*)handle.HANDLE);
    FS_CORE_INFO("Created VertexArray buffer entity: {}", entity);
    return entity;
  }

  VertexArraySystem *VertexArraySystem::GetInstance() {
    if (!ptr) ptr = new VertexArraySystem();
    return ptr;
  }
}