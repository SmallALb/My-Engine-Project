#pragma once


#include "Editor.h"

namespace FISH {
    //编辑顶点命令
    class EditVertexCommand : public Command {
        using vec = std::variant<glm::vec3, glm::vec2, glm::vec4>;
    public:
        EditVertexCommand(ShapePtr shape, VertexType typ, uint32_t idx, vec Data);
        
        bool execute() override;

        bool cancel() override;

    private:
        VertexType fixType;
        vec        data;
        uint32_t   index;
        ShapePtr EditShape{nullptr};
    };
    //创建几何体命令
    class AddShapeCommand : public Command {
    public:
        AddShapeCommand(std::vector<ShapePtr>& ref_, const ShapePtr& ptr_);

        bool execute() override;

        bool cancel() override;
    private:
        ShapePtr    ptr;
        std::vector<ShapePtr>& ref;
    };



    class GeomtryEditor : public Editor {
        friend class AddShapeCommand;

        GeomtryEditor() {}
    public:
        ~GeomtryEditor();

        void OnImGuiRender() override;

        void OnAttach() override;

        void OnUpdate(float dt) override;
    
        static GeomtryEditor* get();
    private:
        void addShape();

        void fixVertex();

        void delShape();
        
        void ediVertex();

    private:
        static GeomtryEditor* mInstance;
        std::vector<ShapePtr>   Shapes;
        ShapePtr                currentEdiShape;
        std::shared_ptr<FrameBuffer>    mFramebuffer;
        std::shared_ptr<Renderstatus>   mStatus;
    };

}