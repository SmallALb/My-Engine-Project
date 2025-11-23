#pragma once

#include "../SparseMap.h"
#include "Component.h"

class ComponentBase {
public:
  virtual ~ComponentBase() = default;
  virtual void erase(uint32_t entity) = 0;
  virtual void erase(uint32_t entity, size_t index) = 0;
  virtual void* get(uint32_t entity, size_t index) = 0;
  virtual size_t getComponentCount(uint32_t entity) const =0;
  virtual void clear() = 0;
};


//组件池（保证内存连续安全）
template<class T>
class API_ ComponentPool : public ComponentBase {
  static_assert(
    std::is_base_of_v<Component, T>, "T must be derived from Component"
  );
  //组件列表
  using EntityComponents = sparse_map<size_t, T>;
public:
  ComponentPool() {}
  //add with an entity ID, and an component ID which give from user  
  template<class ...Args>
  T& add(uint32_t entity, size_t id, Args&&... args) {
    if (!mEntityComponents.contains(entity)) mEntityComponents[entity] = EntityComponents();
    auto& entityComps = mEntityComponents[entity];
    entityComps.emplace(id, std::forward<Args>(args)...);
    mComponentToEntity[id] = entity;
    mSize++;

    return entityComps[id];
  }

  void* get(uint32_t entity, size_t id) override {
    return (void*)(&mEntityComponents[entity][id]);
  }


  size_t getComponentCount(uint32_t entity) const override {
    auto it = mEntityComponents.find(entity);
    return it != mEntityComponents.end() ? it->second.size() : 0;
  }

  void clear() override {
    mEntityComponents.clear();
    mComponentToEntity.clear();
    mSize = 0;
  }
  //移除所有
  void erase(uint32_t entity) override {
    if (!mEntityComponents.contains(entity)) return;
    for (auto [i, e] : mComponentToEntity) if (e == entity){
      mComponentToEntity.erase(i);
      mSize--;
    }
    mEntityComponents.erase(entity);
  }


  virtual void erase(uint32_t entity, size_t index) {
    if (!mEntityComponents.contains(entity) || index >= mEntityComponents[entity].size()) return;
    auto& comps = mEntityComponents[entity];
    comps.erase(index);
    if (comps.empty()) mEntityComponents.erase(entity);
  }



private:
  sparse_map<uint32_t, EntityComponents> mEntityComponents;
  sparse_map<size_t, uint32_t> mComponentToEntity;
  size_t mSize{0};
  size_t MaxSize{1<<3};
};

