#pragma once

#include "../SparseMap.h"
#include "Component.h"

class ComponentBase {
public:
  virtual ~ComponentBase() = default;
  virtual void erase(uint32_t entity) = 0;
  virtual void* get(uint32_t entity) = 0;
  virtual void clear() = 0;
};


//### ComponetPool
//- One entity can map many components
template<class T>
class API_ ComponentPool : public ComponentBase {
  static_assert(
    std::is_base_of_v<Component, T>, "T must be derived from Component"
  );
public:
  using EntityComponent = T; 
public:
  ComponentPool() {}
  //add an entity with an ID, and a component ID provided by the user  
  template<class ...Args>
  T& add(uint32_t entity, Args&&... args) {
    if (!mEntityComponents.contains(entity)) {
      mEntityComponents.emplace(entity, std::forward<Args>(args)...);
      mComponentToEntity[&mEntityComponents[entity]] = entity;
    } else {
      mEntityComponents[entity] = T(std::forward<Args>(args)...);
      mComponentToEntity.erase(&mEntityComponents[entity]);
      mComponentToEntity[&mEntityComponents[entity]] = entity;
    }
    return mEntityComponents[entity];
  }

  void* get(uint32_t entity) override {
    return (void*)(&mEntityComponents[entity]);
  }

  void clear() override {
    mEntityComponents.clear();
    mComponentToEntity.clear();
  }

  void erase(uint32_t entity) override {
    if (!mEntityComponents.contains(entity)) return;
    mComponentToEntity.erase(&mEntityComponents[entity]);
    mEntityComponents.erase(entity);
  }

  size_t size() const {
    return mComponentToEntity.size();
  }
  
private:
  sparse_map<uint32_t, EntityComponent> mEntityComponents;
  std::unordered_map<EntityComponent*, uint32_t> mComponentToEntity;
};

