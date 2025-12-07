#pragma once

#include "../LockFreeQue.h"
#include "ComponentPool.h"

class Registry;

//实体状态
struct EntityState {
  uint32_t id, version;
  uint64_t TypeMask{0};
  bool alive;  
  std::function<void(uint32_t, Registry&)> destoryFunc;
};

//### Registry
class API_ Registry {
public:
  Registry() {}

   // 移动构造函数
  Registry(Registry&& other) noexcept
    : 
      mComponentPools(std::move(other.mComponentPools)),
      mEntityStates(std::move(other.mEntityStates)),
      mNxtId(other.mNxtId.load())
  {
      other.mNxtId = 0;
  }

  // 移动赋值运算符
  Registry& operator=(Registry&& other) noexcept {
    if (this != &other) {
      mComponentPools = std::move(other.mComponentPools);
      mEntityStates = std::move(other.mEntityStates);
      mNxtId.store(other.mNxtId);
      other.mNxtId = 0;
    }
    return *this;
  }


  uint32_t create(const std::function<void(uint32_t, Registry&)>& destoryFunc = nullptr) {
    uint32_t id;
    if (!mFreeList.empty()) {
      mFreeList.pop(id);
      mEntityStates[id].alive = true;
      mEntityStates[id].destoryFunc = destoryFunc; 
    }
    else {
      std::lock_guard<std::mutex> lock(mRegistryMutex);
      id = mNxtId++;
      mEntityStates[id] = EntityState{id, 0, 0, true, destoryFunc};
    }
    return id;
  }

  void destory(uint32_t entity) {
    mRegistryMutex.lock();
    auto& record = mEntityStates[entity];
    record.alive = false;
    record.version++;
    
    for (auto& pool : mComponentPools) if (pool) pool->erase(entity);

    mEntityStates[entity].TypeMask = 0;
    if (mEntityStates[entity].destoryFunc) mEntityStates[entity].destoryFunc(entity, *this);
    mRegistryMutex.unlock();
    mFreeList.push(entity);
  }
  //添加
  template<class T, class ...Args>
  T& add(uint32_t entity, Args&&... args) {
    uint8_t typeId = Registry::getComponentTypeId<T>();

    std::lock_guard<std::mutex> lock(mRegistryMutex);
    if (typeId >= mComponentPools.size()) 
      mComponentPools.resize(typeId + 1);
    if (!mComponentPools[typeId])
      mComponentPools[typeId] = std::move(std::make_unique<ComponentPool<T>>());
    mEntityStates[entity].TypeMask |= (1ull << typeId);
    auto& pool = static_cast<ComponentPool<T>&>(*mComponentPools[typeId]);
    return pool.add(entity, std::forward<Args>(args)...);
  }

  template<class T>
  T& get(uint32_t entity) {
    uint8_t typId = Registry:: getComponentTypeId<T>();
    auto& pool = static_cast<ComponentPool<T>&>(*mComponentPools[typId]);
    return  static_cast<T&>(*(T*)pool.get(entity));
  }

  template<class T>
  const T& get(uint32_t entity) const {
    uint8_t typId = Registry:: getComponentTypeId<T>();
    auto& pool = static_cast<ComponentPool<T>&>(*mComponentPools[typId]);
    return  static_cast<T&>(*(T*)pool.get(entity));
  }

  //Checks for existence of T in the entity
  template<class T> 
  bool has(uint32_t entity) const {
      uint8_t typId = Registry::getComponentTypeId<T>();
      auto it = mEntityStates.find(entity); 
      return it != mEntityStates.end() ? 
        it->second.TypeMask & (1ull << typId) :
        false;
  }


  //Checks for existence of entity
  bool has_entity(uint32_t entity) const {
    return mEntityStates.contains(entity);
  }

  //Deletes all the components about one type for entity
  template<class T>
  void erase(uint32_t entity) {
    uint8_t typeId = Registry::getComponentTypeId<T>();
    std::lock_guard<std::mutex> lock(mRegistryMutex);
    if (typeId < mComponentPools.size() && mComponentPools[typeId]) {
      mComponentPools[typeId]->erase(entity);
      mEntityStates[entity].TypeMask &= ~(1ull << typeId);
    }
  }

  const sparse_map<uint32_t, EntityState>& getCreatedEntitys() const {
    return mEntityStates;
  }

private:
  template<class T>
  static uint8_t getComponentTypeId() {
    static uint8_t typeId = mNxtTypeId++;
    return typeId;
  }

  static inline uint8_t mNxtTypeId = 0;
private:
  std::vector<std::unique_ptr<ComponentBase>> mComponentPools;
  sparse_map<uint32_t, EntityState> mEntityStates;
  mutable std::mutex mRegistryMutex;
  LockFreeQue<uint32_t> mFreeList;
  std::atomic<uint32_t> mNxtId = 0;
};