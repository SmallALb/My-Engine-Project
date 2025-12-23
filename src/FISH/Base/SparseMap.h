#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <cassert>

//稀疏集合
template<class EntityType = uint32_t, class T = size_t, size_t PAGE_SHIFT = 4>
class sparse_map {
  static_assert(std::is_unsigned_v<EntityType>, "EntityType must be an unsigned integer!");
//声明类型
public:
  using EntityType_ = EntityType;
  using DataType_ = T;
  using SizeType_ = size_t;

  using Entry = std::pair<EntityType_, DataType_>;

private:
  //基本数据声明
  static constexpr int32_t INVALID_INDEX = -1;
  static constexpr SizeType_ PAGE_SIZE = (1<<PAGE_SHIFT);
  static constexpr SizeType_ PAGE_MASK = PAGE_SIZE-1;

public:
  sparse_map() {

  }

  sparse_map(const sparse_map& other) 
    : mDense(other.mDense), mSize(other.mSize), mSparse(other.mSparse) {
    other.isCopy = 1;
  }

  sparse_map(sparse_map&& other) noexcept 
    : mSparse(std::move(other.mSparse)),
      mDense(std::move(other.mDense)),
      mSize(other.mSize) {
    other.mSize = 0;
    other.mSparse.clear();
  }

  sparse_map& operator=(const sparse_map& other) {
    if (this != &other) {
      // 先清理当前内存
      clear_memory();
      
      mDense = other.mDense;
      mSize = other.mSize;
      other.setCopy();
      mSparse.resize(other.mSparse.size(), nullptr);
      for (SizeType_ i = 0; i < other.mSparse.size(); ++i) {
        if (other.mSparse[i]) mSparse[i] = other.mSparse[i];
      }
    }
    return *this;
  }

  sparse_map& operator=(sparse_map&& other) noexcept {
    if (this != &other) {
      clear_memory();
      mSparse = std::move(other.mSparse);
      mDense = std::move(other.mDense);
      mSize = other.mSize;
      other.mSize = 0;
      other.mSparse.clear();
    }
    return *this;
  }

  ~sparse_map() {
    clear_memory();
  }
  
  bool empty() const {return mSize == 0;}

  SizeType_ size() const {return mSize;}

  SizeType_ capacity() const {return mDense.capacity();}

  bool contains(EntityType_ entity) const {
    //第i页
    const SizeType_ pageIndex = entity >> PAGE_SHIFT;
    //页内偏移
    const SizeType_ pageOffset = entity & PAGE_MASK;

    if (pageIndex >= mSparse.size() || !mSparse[pageIndex]) return false;
    //取出在密集数组中的数据
    const int32_t dense_index = mSparse[pageIndex][pageOffset];
    return dense_index != INVALID_INDEX &&  
          dense_index < mSize && 
          mDense[dense_index].first == entity;
  }

  void reverse(SizeType_ capacity) {
    mDense.reserve(capacity);
    //预留两倍
    SizeType_ maxEntity = capacity << 1;
    //计算至少需要页
    SizeType_ required_pages = (maxEntity + PAGE_MASK) / PAGE_SIZE;
    if (mSparse.size() < required_pages) mSparse.resize(required_pages); 
  }
  
  DataType_& insert(EntityType_ entity, DataType_ data = DataType_{}) {
    if (contains(entity)) return mDense[mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK]].second;
    //让此实体合法
    make_valid_sparePage(entity);
    if (mSize < mDense.size()) mDense[mSize] = Entry{entity, data};
    else mDense.push_back(Entry{entity, data});

    mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK] = mSize;
    mSize++;

    return mDense[mSize-1].second;
  }

  template<class  ...Args>
  DataType_& emplace(EntityType_ entity, Args&&... args) {
    if (contains(entity)) return mDense[mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK]].second;
    make_valid_sparePage(entity);
    if (mSize < mDense.size()) mDense[mSize] = Entry{entity, DataType_(std::forward<Args>(args)...)};
    mDense.emplace_back(entity, T(std::forward<Args>(args)...));

    mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK] = mSize;
    mSize++;

    return mDense[mSize-1].second;
  }
  
  DataType_& get(EntityType_ entity) {
    //在引用调用中必须保证存在该实体
    make_valid_sparePage(entity);
    int32_t dense_index = mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK];
    //不存在则
    if (dense_index == INVALID_INDEX) {
      if (mSize < mDense.size()) {
        mDense[mSize] = Entry{entity, DataType_{}};
        dense_index = mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK] = mSize;
      }
      else {
        mDense.emplace_back(entity, DataType_{});
        dense_index = mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK] = mDense.size() - 1;
        mSize++;
      }
    }

    return mDense[dense_index].second;
  }

  const DataType_& get(EntityType_ entity) const {
    assert(!contains(entity));
    return mDense[mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK]].second;
  }

  void erase(EntityType_ entity) {
    if (!contains(entity)) return;
    const SizeType_ page_index = entity >> PAGE_SHIFT;
    const SizeType_ page_offset = entity & PAGE_MASK;
    //取出要删除的密集数组下标
    const int32_t dense_index = mSparse[page_index][page_offset];
    if (dense_index != mSize - 1) {
      const EntityType_ lstEntity = mDense[mSize - 1].first;
      //设置要被删除的实体的密集数组位置变为back的位置
      mDense[dense_index] = std::move(mDense[mSize - 1]);
      mSparse[lstEntity >> PAGE_SHIFT][lstEntity & PAGE_MASK] = dense_index;
    }
    //稀疏数组置为-1
    mSparse[page_index][page_offset] = INVALID_INDEX;
    mSize--;
  }

  void clear() {
    for (SizeType_ i = 0; i< mSize; i++) {
      const EntityType_ entity = mDense[i].first;
      mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK] = INVALID_INDEX;
    }
    mSize = 0;
    //不直接清理内存
  }

  DataType_& operator[] (EntityType_ entity) {
    return get(entity);
  }

  const DataType_& operator[] (EntityType_ entity) const {
    return get(entity);
  }

  //Iterator
  auto begin() {return mDense.begin();}
  auto begin() const {return mDense.begin();}

  auto end() {return mDense.begin() + mSize;}
  auto end() const {return mDense.begin() + mSize;}

  //find
  auto find(EntityType_ entity) {
    if (!contains(entity)) return end();
    return begin() + mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK];
  }

  auto find(EntityType_ entity) const{
    if (!contains(entity)) return end();
    return begin() + mSparse[entity >> PAGE_SHIFT][entity & PAGE_MASK];
  }

  EntityType get_entity(SizeType_ index) const {
    assert(index >= mSize);
    return mDense[index].first;
  }

  DataType_& get_data(SizeType_ index) {
    assert(index >= mSize);
    return mDense[index].second;
  }

  DataType_& get_data(SizeType_ index) const {
    assert(index >= mSize);
    return mDense[index].second;
  }

  SizeType_ get_page_count() const {
    return mSparse.size();
  }

  SizeType_ get_allocated_sparse_size() const {
    SizeType_ t = 0;
    for (const auto& page : mSparse) if (page) t += PAGE_SIZE;
    return t;
  }

private:
  void setCopy() const {isCopy = 1;}

  void make_valid_sparePage(EntityType_ entity) {
    const SizeType_ page_index = entity >> PAGE_SHIFT;
    //无此页则添加
    if (page_index >= mSparse.size()) mSparse.resize(page_index+1, nullptr);
    //页面为空则新建页面并填充不合法下标
    if (!mSparse[page_index]) {
      mSparse[page_index] = new int32_t[PAGE_SIZE];
      std::fill_n(mSparse[page_index], PAGE_SIZE, INVALID_INDEX);
    }
  }

  void clear_memory() {
    if (isCopy) return;
    for (auto page : mSparse) if (page != nullptr) 
      delete[] page;
    mSparse.clear();
  }
  
private:
  std::vector<int32_t*> mSparse{};
  std::vector<Entry> mDense;
  SizeType_ mSize{0};
  mutable bool isCopy{0};
};