#pragma once

#include "ComponentPool.h"

//实体状态
struct EntityState {
  uint32_t id, version;
  bool alive;
  
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
      mFreeList(std::move(other.mFreeList)),
      mNxtId(other.mNxtId),
      mComponentMasks(std::move(other.mComponentMasks)) {
    other.mNxtId = 0;
  }

  // 移动赋值运算符
  Registry& operator=(Registry&& other) noexcept {
    if (this != &other) {
      mComponentPools = std::move(other.mComponentPools);
      mEntityStates = std::move(other.mEntityStates);
      mFreeList = std::move(other.mFreeList);
      mNxtId = other.mNxtId;
      mComponentMasks = std::move(other.mComponentMasks);
      other.mNxtId = 0;
    }
    return *this;
  }


  uint32_t create() {
    uint32_t id;
    //有被释放的实体
    if (!mFreeList.empty()) {
      id = mFreeList.back();
      mFreeList.pop_back();
      mEntityStates[id].alive = true;
    }
    //没有被释放的实体
    else {
      id = mNxtId++;
      if (id >= mEntityStates.size()) {
        mEntityStates.resize(id + 1);
        mComponentMasks.resize(id + 1);
      }
      mEntityStates[id] = EntityState{id, 0, true};
    }
    return id;
  }
  //销毁
  void destory(uint32_t entity) {
    auto& record = mEntityStates[entity];
    record.alive = false;
    record.version++;
    
    for (auto& pool : mComponentPools) if (pool) pool->erase(entity);

    mComponentMasks[entity] = 0;
    mFreeList.push_back(entity);
  }
  //添加
  template<class T, class ...Args>
  T& add(uint32_t entity, size_t id, Args&&... args) {
    uint8_t typeId = Registry::getComponentTypeId<T>();
    if (typeId >= mComponentPools.size()) 
      mComponentPools.resize(typeId + 1);
    if (!mComponentPools[typeId])
      mComponentPools[typeId] = std::move(std::make_unique<ComponentPool<T>>());
    mComponentMasks[entity] |= (1ull << typeId);
    auto& pool = static_cast<ComponentPool<T>&>(*mComponentPools[typeId]);
    return pool.add(entity, id, std::forward<Args>(args)...);
  }

  //Gets a component by the given id
  template<class T>
  T& get(uint32_t entity, size_t id) {
    uint8_t typId = Registry:: getComponentTypeId<T>();
    auto& pool = static_cast<ComponentPool<T>&>(*mComponentPools[typId]);
    return  static_cast<T&>(*(T*)pool.get(entity, id));
  }


  //查看是否拥有
  template<class T> 
  bool has(uint32_t entity) const {
      uint8_t typId = Registry::getComponentTypeId<T>();
      return mComponentMasks[entity] & (1ull << typId);
  }

  

  //Deletes all the components about one type for entity
  template<class T>
  void erase(uint32_t entity) {
    uint8_t typeId = Registry::getComponentTypeId<T>();
    if (typeId < mComponentPools.size() && mComponentPools[typeId]) {
      mComponentPools[typeId]->erase(entity);
      mComponentMasks[entity] &= ~(1ull << typeId);
    }
  }

  //Deletes one component for entity by the given componentID 
  template<class T>
  void erase(uint32_t entity, size_t index) {
    uint8_t typId = Registry::getComponentTypeId<T>();
    if (mComponentPools.size() <= typId || !mComponentPools[typId]->getComponentCount(entity)) return;
    mComponentPools[typId]->erase(entity, index);
    if (!mComponentPools[typId]->getComponentCount(entity)) 
      mComponentMasks[entity] &= ~(1ull << typId);
  }

  template<class T>
  size_t size(uint32_t entity) const {
    uint8_t typId = Registry::getComponentTypeId<T>();
    return has<T>(entity) ? 
      mComponentPools[typId]->getComponentCount(entity) : 0;
  }

  template<class T>
  const sparse_map<size_t, uint32_t>& get_Component_to_entityID() const {
    uint8_t typId = Registry::getComponentTypeId<T>();
    return mComponentPools[typId] ? static_cast<ComponentPool<T>&>(*mComponentPools[typId]).get_all_ComponentID_to_entityID() : sparse_map<size_t, uint32_t>{}; 
  }

  template<class T>
  ComponentPool<T>::EntityComponents& getComponents(uint32_t entity) {
    uint8_t typId = Registry::getComponentTypeId<T>();
    return mComponentPools[typId] ? 
      static_cast<ComponentPool<T>&>(*mComponentPools[typId]).getComponents(entity) : 
      ComponentPool<T>::EMPTYMAP;
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
  std::vector<EntityState> mEntityStates;
  std::vector<uint32_t> mFreeList;
  uint32_t mNxtId = 0;
  std::vector<uint64_t> mComponentMasks;
};