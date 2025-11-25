#pragma once

#include <atomic>

//## Lock Free Que
template<class T>
class LockFreeQue {
public:
  //DATA
  using DataType_ = T;
  //Atomic
  template<class PTR>
  using AtoTyp_ = std::atomic<PTR>;
private:
  //Queue Node
  struct Node {
    DataType_ data;
    AtoTyp_<Node*> next{nullptr};
    Node() {}
    Node(const DataType_& d): data(d) {}
  };

  AtoTyp_<Node*> head;
  AtoTyp_<Node*> tail;
public:
  LockFreeQue() {
    Node* Beg = new Node();
    head.store(Beg);
    tail.store(Beg);
  }
  //delete Copy structure
  LockFreeQue(const LockFreeQue&) = delete;
  LockFreeQue& operator=(const LockFreeQue&) = delete;
  
  ~LockFreeQue() {
    if (!head.load()) return;
    DataType_ Dummy;
    while(pop(Dummy));
    delete head.load();
  }

  void push(const DataType_& val) {
    if (head.load(std::memory_order_acquire) == nullptr) return; // Queue had been destructure
    Node* newNode = new Node(val);
    Node* currentT = nullptr;
    Node* currentNxt = nullptr;

    while(1) {
      currentT = tail.load(std::memory_order_acquire);
      currentNxt = currentT->next.load(std::memory_order_acquire);
      if (currentT == tail.load(std::memory_order_acquire))
        //if the tail's next is null, it means we can add new node after the tail, set tail->next to a new Node;
        if (!currentNxt) {if (currentT->next.compare_exchange_weak(currentNxt, newNode, std::memory_order_release, std::memory_order_relaxed))
            //if the operation succeeds, set tail to a new Node and end the function;
            return tail.compare_exchange_weak(currentT, newNode, std::memory_order_release), void();
        //if the tail's next not null, help other threads which are adding new nodes to proceed;
        } else tail.compare_exchange_weak(currentT, currentNxt, std::memory_order_release);
    }
  }

  bool pop(DataType_& val) {
    if (head.load(std::memory_order_acquire) == nullptr) return false; // Queue had been destructure
    Node* currentH = nullptr;
    Node* currentT = nullptr;
    Node* currentNxt = nullptr;

    while(1) {
      currentH = head.load(std::memory_order_acquire);
      currentT = tail.load(std::memory_order_acquire);
      currentNxt = currentH->next.load(std::memory_order_acquire);

      if (currentH == head.load(std::memory_order_acquire)) 

        if (currentH == currentT) {
          //empty
          if (currentNxt = nullptr) return false;
          //help other threads which are erasing nodes to proceed
          tail.compare_exchange_weak(currentT, currentNxt, std::memory_order_release);
        }
        else {
          //give the erased data for reference;
          if (currentNxt) val = currentNxt->data;
          //try to set head to the next node;
          if (head.compare_exchange_weak(currentH, currentNxt, std::memory_order_release)) {
            delete currentH;
            return true;
          }
        }
    }
  }

  bool empty() const {
    return head.load(std::memory_order_acquire)->next.load(std::memory_order_acquire) == nullptr;
  }

};