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
  using EntityComponents = std::vector<T>;
public:
  ComponentPool() {}

  template<class ...Args>
  T& add(uint32_t entity, Args&&... args) {
    if (!mEntityComponents.contains(entity)) mEntityComponents[entity] = EntityComponents();

    auto& entityComps = mEntityComponents[entity];
    entityComps.emplace_back(std::forward<Args>(args)...);
    

    if (mSize >= mComponents.size()) {
      auto newCapacity = (std::max)(MaxSize, mComponents.capacity());
      MaxSize = newCapacity;
      mComponents.resize(newCapacity, nullptr);
      mComponentToEntity.resize(newCapacity);
      MaxSize <<= 1;
    }

    mComponents[mSize] = &entityComps.back();
    mComponentToEntity[mSize] = entity;
    mSize++;

    return entityComps.back();
  }

  void* get(uint32_t entity, size_t index) override {
    return (void*)(&mEntityComponents[entity][index]);
  }


  size_t getComponentCount(uint32_t entity) const override {
    auto it = mEntityComponents.find(entity);
    return it != mEntityComponents.end() ? it->data.size() : 0;
  }

  void clear() override {
    mEntityComponents.clear();
    mComponents.clear();
    mComponentToEntity.clear();
    mSize = 0;
    MaxSize = 1 << 3;
  }
  //移除所有
  void erase(uint32_t entity) override {
    if (!mEntityComponents.contains(entity)) return;
    
    for (size_t i=0; i<mSize;) if (mComponentToEntity[i] == entity) {
      mComponents[i] = (i < mSize - 1) ? mComponents[mSize-1] : mComponents[i];
      mComponentToEntity[i] = (i < mSize - 1) ? mComponentToEntity[mSize - 1] : mComponentToEntity[i];
      mSize--;
    } else i++;
    
    mEntityComponents.erase(entity);
  }


  virtual void erase(uint32_t entity, size_t index) {
    if (!mEntityComponents.contains(entity) || index >= mEntityComponents[entity].size()) return;
    auto& comps = mEntityComponents[entity];

    T* targetComp = &comps[index];
    for (size_t i=0; i<mSize; i++) if (mComponents[i] == targetComp) {
      mComponents[i] = (i < mSize - 1) ? mComponents[mSize-1] : mComponents[i];
      mComponentToEntity[i] = (i < mSize - 1) ? mComponentToEntity[mSize - 1] : mComponentToEntity[i];
      mSize--;
      break;
    }
    
    comps.erase(comps.begin() + index);

    if (comps.empty()) mEntityComponents.erase(entity);
  }

public:
  //迭代器
  class Iterator {
  public:
    Iterator(ComponentPool<T>* pool, size_t index):mPool(pool), mIndex(index) {}

    T& operator *() {return mPool->mComponents[mIndex];}

    Iterator& operator ++() {mIndex++; return *this;}

    bool operator ==(Iterator&& other) const {return mIndex == other.mIndex;}

    bool operator !=(Iterator&& other) const {return mIndex != other.mIndex;}
  private:
    ComponentPool<T>* mPool;
    size_t mIndex;
  };

  Iterator begin() {return Iterator(this, 0); }
  Iterator end() {return Iterator(this, mSize);}
private:
  sparse_map<uint32_t, EntityComponents> mEntityComponents;
  std::vector<T*> mComponents;
  std::vector<uint32_t> mComponentToEntity;
  size_t mSize{0};
  size_t MaxSize{1<<3};
};

