#pragma once
#include <utility>
#include "Core.h"

namespace FISH {
    // API_ void* Alloc(size_t size);

    // API_ void Free(void* ptr);

    // //构建对象
    // template <typename T, typename... Args>
    // T* Create(Args&&... args) {
    //     void* mem = Alloc(sizeof(T));
    //     return new (mem) T(std::forward<Args>(args)...);
    // }

    // // 销毁对象（调用析构函数 + 释放内存）
    // template <typename T>
    // void Destroy(T* obj) {
    //     if (obj) {
    //         obj->~T();
    //         Free(obj);
    //     }
    // }

}