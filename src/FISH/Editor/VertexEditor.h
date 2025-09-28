#pragma once

#include "FISH/Base/BaseTree.h"
#include "Editor.h"

namespace FISH {

    using vertexData = std::pair<glm::vec3, int>;


    struct VertexHash {
        std::size_t operator()(const vertexData& p) const {
            Vec3Hash _hash;
            return _hash(p.first);
        };
    };

    struct VertexEqual {
        bool operator() (const vertexData& a, const vertexData& b) const {
            Vec3Equal _equal;
            return _equal(a.first, b.first);
        }
    };
    //编辑顶点命令
    class EditVertexCommand : public Command {
        using vec = std::variant<glm::vec3, glm::vec2, glm::vec4>;
    public:
        EditVertexCommand(ShapePtr shape, VertexType typ, uint32_t idx, vec Data, vec oldData_);
        
        bool execute() override;

        bool cancel() override;

    private:
        VertexType fixType;
        vec        data;
        vec        olddata;
        uint32_t   index;
        ShapePtr EditShape{nullptr};
    };



    //几何体顶点编辑器
    class GeomtryEditor : public Editor {
        friend class AddShapeCommand;

        GeomtryEditor() {}
    public:
        ~GeomtryEditor();

        void OnImGuiRender() override;

        void OnAttach() override;

        void OnUpdate(float dt) override;
    
        static GeomtryEditor* get();
    //渲染imgui的逻辑窗口等
    private:            
        void renderVertexEdit();
        
        void renderShapeViewer();

        void MousePicking();

        int findClosestVertex(const glm::vec3& ratO, const glm::vec3& rayD);

        float distanceToNearPoint(const glm::vec3& point, const glm::vec3& lineO, const glm::vec3& lineD);

        void buildTree();
    private:
        static GeomtryEditor*           mInstance;
        ShapePtr                        currentEdiShape;
        std::shared_ptr<FrameBuffer>    mFramebuffer;
        std::shared_ptr<Renderstatus>   mStatus;
        FISH::ShaderPtr                 mRenderShared;
        CameraPtr                       mCamera;
        int                             selectedVertex{0};
        bool                            mfocusedTag{0};
        glm::vec3                       mRayOrigin;
        glm::vec3                       mRayDirection;
        glm::vec2                       mLstMousePos;
        bool                            mIsMouseOverViewport{false};
        float                           PickAccuracy{0.1};
        //八叉树
        std::shared_ptr<OcTree<vertexData, VertexHash, VertexEqual>>         ChoiceTree{nullptr};
        
    };

}