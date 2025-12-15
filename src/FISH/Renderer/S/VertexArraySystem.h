#pragma once

namespace FISH {
  

  class VertexArraySystem : public FISH_System {
    VertexArraySystem() {}
  public:
    static VertexArraySystem* getInstance();

    void bind(uint32_t entity);

    void addBuffer(uint32_t Ventity, uint32_t Bentity);

    void addBuffer(uint32_t Ventity, const std::initializer_list<uint32_t>& Bentitys);

    void setIndex(uint32_t Ventity, uint32_t Bentity);

    void destoryVertexArray(uint32_t entity);

    void OnDetach() override;

    uint32_t create();
    
    static VertexArraySystem* GetInstance();
  private:
    static VertexArraySystem* ptr; 
  }; 

}