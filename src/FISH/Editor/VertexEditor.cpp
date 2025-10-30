#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/Renderer/Renderstatus.h"
#include "FISH/Object/Camera.h"
#include "FISH/Base/HashTable.h"

#include "FISH/Physics/Ray/Ray.h"
#include "FISH/Renderer/Shader.h"
#include "FISH/input.h"
#include "VertexEditor.h"
#include "FISH/ImGui/ImGuiLayer.h"

#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Object/perspectiveCamera.h"

namespace FISH {

    bool divefunc(const BlankSpace& space, const vertexData& data){
        auto point = data.first;
        return point.x >= space.Min.x && point.x <= space.Max.x &&
            point.y >= space.Min.y && point.y <= space.Max.y &&
            point.z >= space.Min.z && point.z <= space.Max.z;
    };

    static glm::vec3 originalPosition;
    static glm::vec4 originalColor;
    static glm::vec3 originalNormal;
    static glm::vec2 originalUV;
    static bool isEditing = false;

    GeomtryEditor* GeomtryEditor::mInstance = nullptr;

    GeomtryEditor::~GeomtryEditor() {
    }

    void GeomtryEditor::OnImGuiRender() {
        UI::Begin("Editor");
        renderVertexEdit();
        UI::End();

        renderShapeViewer();
    }

    void GeomtryEditor::OnAttach() {
        //新建帧
        mFramebuffer.reset(FrameBuffer::Create(2250, 1750));
        //新建渲染状态
        mStatus.reset(Renderstatus::CreateRenderstatus());
        //新建相机
        mCamera.reset(new FISH::perspectiveCamera("GEditor camera"));
        mCamera->setPosition({0.0, 0.0, 2.0});
        mCamera->setLookAt(glm::vec3{0.0});
        //启动状态
        mStatus->enablestatus(StatusType::CleanColor);
        mStatus->enablestatus(StatusType::CleanDepth);
        //渲染状态
        mRenderShared.reset(Shader::CreateShader());
        mRenderShared->readVertexShader("sharders/EnginRenderShader/VertexHightLightVer.glsl");
        mRenderShared->readFragmentShader("sharders/EnginRenderShader/VertexHightLightFrg.glsl");
        mRenderShared->CompileLink();
        
        ChoiceTree.reset(new OcTree<vertexData, VertexHash, VertexEqual>(
            {-10, -10, -10}, {10, 10, 10}
        ));

        buildTree();

    }

    void GeomtryEditor::OnUpdate(float dt) {

        mFramebuffer->bind();
        mStatus->disablestatus(StatusType::DepthTest);
        mStatus->disablestatus(StatusType::Blend);
        Static_PtrCastTo<perspectiveCamera>(mCamera)->init(
            60.0,
            (float)mFramebuffer->GetWidth()/mFramebuffer->GetHeight(),
            0.01,
            100.0f
        );

        mFramebuffer->updateCleanStatus(mStatus->getCleanstatuss());

        mRenderShared->Begin();
        mRenderShared->setInt("uSelectedVertexIndex", selectedVertex);
        mRenderShared->setMat4("view", mCamera->getViewMatrix());
        mRenderShared->setMat4("projection", mCamera->getProjectMatrix());
        mRenderShared->setMat4("model", glm::mat4(1.0));
        mRenderShared->setVector3("moveUV", {0.0, 0.0, 0.0});
        mRenderShared->setVector4("Incolor", {0.0 ,0.0, 1.0, 1.0});
        if (currentEdiShape){
            currentEdiShape->render(LINES);
            currentEdiShape->render(POINTS);
        }
        mRenderShared->End();

        mStatus->enablestatus(StatusType::DepthTest);
        mStatus->enablestatus(StatusType::Blend);
        mFramebuffer->unbind();

        if (!mfocusedTag && Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_Z))
            {FS_INFO("Cancel one command!") ;Editor::mEditorCommands.cancel();}
        
    }

    GeomtryEditor* GeomtryEditor::get() {
        if (mInstance == nullptr) mInstance = new GeomtryEditor();
        return mInstance;
    }

    void GeomtryEditor::renderVertexEdit() {
        mfocusedTag = ImGui::IsWindowFocused();
        if (ImGui::IsWindowFocused() && ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_Z)) 
            {FS_INFO("Cancel one command!"); Editor::mEditorCommands.cancel();}
        


        if (currentEdiShape && ImGui::CollapsingHeader("Vertex Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto& vertexArray = currentEdiShape->getVertexArrary();
            uint32_t vertexCount = vertexArray->GetVertexBuffers()[0]->GetVertexCount();
            
            // 顶点选择器
            ImGui::SliderInt("Vertex Index", &selectedVertex, 0, vertexCount - 1);
            
            if (selectedVertex >= 0 && selectedVertex < vertexCount) {
                // 获取当前顶点数据
                glm::vec3 position = vertexArray->GetVertexPosition(selectedVertex);
                glm::vec4 color = vertexArray->GetVertexColor(selectedVertex);
                glm::vec3 normal = vertexArray->GetVertexNormal(selectedVertex);
                glm::vec2 uv = vertexArray->GetVertexUV(selectedVertex);
                
                // 位置编辑
                if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
                    if (!isEditing) {
                        // 开始编辑时保存原始位置
                        originalPosition = vertexArray->GetVertexPosition(selectedVertex);
                        isEditing = true;
                    }
                    // 实时更新显示
                    vertexArray->UpdateVertexPosition(selectedVertex, position);
                }
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    if (isEditing) {
                        auto command = std::make_shared<EditVertexCommand>(
                            currentEdiShape, 
                            VertexType::Position, 
                            selectedVertex, 
                            position,
                            originalPosition,
                            ChoiceTree
                        );
                        FS_INFO("Add position Vertex Fix Command!");
                        Editor::mEditorCommands.PushCommand(command);
                        isEditing = false;
                    }
                }

                // 颜色编辑
                if (ImGui::ColorEdit4("Color", glm::value_ptr(color))) {
                    if (!isEditing) {
                        // 开始编辑时保存原始颜色
                        originalColor = vertexArray->GetVertexColor(selectedVertex);
                        isEditing = true;
                    }
                    // 实时更新显示
                    vertexArray->UpdateVertexColor(selectedVertex, color);
                }
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    if (!isEditing) {
                        auto command = std::make_shared<EditVertexCommand>(
                            currentEdiShape, 
                            VertexType::Color, 
                            selectedVertex, 
                            color,
                            originalColor,
                            ChoiceTree
                        );
                        FS_INFO("Add color Vertex Fix Command!");
                        Editor::mEditorCommands.PushCommand(command);
                        isEditing = false;
                    }
                }

                // 法线编辑
                if (ImGui::DragFloat3("Normal", glm::value_ptr(normal), 0.01f, -1.0f, 1.0f)) {
                    if (!isEditing) {
                        // 开始编辑时保存原始法线
                        originalNormal = vertexArray->GetVertexNormal(selectedVertex);
                        isEditing = true;
                    }
                    // 实时更新显示
                    vertexArray->UpdateVertexNormal(selectedVertex, normal);
                }
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    if (!isEditing) {
                        auto command = std::make_shared<EditVertexCommand>(
                            currentEdiShape, 
                            VertexType::Normal, 
                            selectedVertex, 
                            normal,
                            originalNormal,
                            ChoiceTree
                        );
                        FS_INFO("Add normal Vertex Fix Command!");
                        Editor::mEditorCommands.PushCommand(command);
                        isEditing = false;
                    }
                }

                // UV编辑
                if (ImGui::DragFloat2("UV", glm::value_ptr(uv), 0.01f, 0.0f, 1.0f)) {
                    if (!isEditing) {
                        // 开始编辑时保存原始UV
                        originalUV = vertexArray->GetVertexUV(selectedVertex);
                        isEditing = true;
                    }
                    // 实时更新显示
                    vertexArray->UpdateVertexUV(selectedVertex, uv);
                }
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    if (isEditing) {
                        auto command = std::make_shared<EditVertexCommand>(
                            currentEdiShape, 
                            VertexType::UV, 
                            selectedVertex, 
                            uv,
                            originalUV,
                            ChoiceTree
                        );
                        FS_INFO("Add uv Vertex Fix Command!");
                        Editor::mEditorCommands.PushCommand(command);
                        isEditing = false;
                    }
                }
            } 
        }
    }

    void GeomtryEditor::renderShapeViewer() {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Viewer");
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        //防止过小 & 隐藏导致的报错
        if (viewportSize.x > 0.0 && viewportSize.y > 0.0){
            mFramebuffer->resize(viewportSize.x, viewportSize.y);
            ImGui::Image(
                mFramebuffer->GetColorAttachmentID(), 
                viewportSize,
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }

        //绕着目标物体旋转
        if (ImGui::IsWindowFocused() && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
            ImVec2 mousePos = ImGui::GetMousePos();
            float dx = mousePos.x - mLstMousePos.x, dy = mousePos.y - mLstMousePos.y;

            glm::mat4 yawRot = glm::rotate(glm::mat4(1.0), -dx * 0.005f, mCamera->getUp());
            glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0), -dy * 0.005f, mCamera->getLeft());

            glm::vec4 tmpPos = yawRot * pitchRot * glm::vec4(mCamera->getPosition(), 1.0);
            mCamera->setPosition(glm::vec3(tmpPos));
            mCamera->setLookAt(glm::vec3{0.0});

            if (fabs(io.MouseWheel) > 0.0f){
                auto currentPos = mCamera->getPosition();
                currentPos +=  mCamera->getFront() * io.MouseWheel * 0.005f;
                mCamera->setPosition(currentPos);
            }
        }


        ImGui::End();
        MousePicking();

    }

    void GeomtryEditor::MousePicking() {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Viewer");
        mIsMouseOverViewport = ImGui::IsWindowHovered();
        ImVec2 mousePos = ImGui::GetMousePos();
        mLstMousePos = {mousePos.x, mousePos.y};
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImGui::End();

        if (mIsMouseOverViewport && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImGui::Begin("Viewer");

            ImGui::End();

            ImVec2 viewPos = {
                mousePos.x - windowPos.x,
                mousePos.y - windowPos.y
            };

            FS_INFO("Mx: {}, My: {}", viewPos.x, viewPos.y);
            auto ndcpos = ToNDC({viewPos.x, viewPos.y}, mFramebuffer->GetWidth(), mFramebuffer->GetHeight());
            glm::vec4 rayClip{ndcpos.x, ndcpos.y, 1.0f, 1.0f};

            auto invM = glm::inverse(mCamera->getProjectMatrix() * mCamera->getViewMatrix());
            glm::vec4 rayWorld = invM * rayClip;
            //透视修正
            rayWorld /= rayWorld.w;

            auto RayO = mCamera->getPosition();  
            auto RayD = glm::normalize(glm::vec3(rayWorld) - RayO);
            
            int closetVertex = findClosestVertex(RayO, RayD);

            selectedVertex = closetVertex != -1 ? closetVertex : selectedVertex;
        }
    }

    int GeomtryEditor::findClosestVertex(const glm::vec3 &rayO, const glm::vec3 &rayD) {
        int closeVertex = -1;
        float pDis = 0.0;
        //查询回调函数
        auto queryCallback = [&, this](
            const std::unordered_set<vertexData, VertexHash, VertexEqual>& points, 
            int& close, 
            float& distance) {
                std::vector<vertexData> Points{points.begin(), points.end()};
                // std::sort(Points.begin(), Points.end(), [&, this](auto& a, auto& b) {
                //     auto& pa = a.first;
                //     auto& pb = b.first;
                //     return glm::length(pa - rayO) < glm::length(pb - rayO); 
                // });
                for (auto& point : Points) 
                    if (RayCollisionAtPoint({rayO, rayD}, point.first, PickAccuracy))  {
                        close = point.second;
                        return true;
                    }
            return false;
        };
        //重置划分函数为按照光线划分 此处中Vertex仅仅作为占位
        ChoiceTree->setDividefunc([&](const BlankSpace& space, const vertexData& vertex) {
            return RayCollisionAtBox({rayO, rayD}, space.Min, space.Max);
        });

        ChoiceTree->queryWithSet({glm::vec3(1.0), -1}, 1, queryCallback,
            closeVertex,
            pDis
        );


        return closeVertex;
    }

    float GeomtryEditor::distanceToNearPoint(const glm::vec3 &point, const glm::vec3 &lineO, const glm::vec3 &lineD) {
        auto pointToRayOrigin = point - lineO;
        float projectLength = glm::dot(pointToRayOrigin, lineD);

        if (projectLength < 0) return glm::length(pointToRayOrigin);
        
        glm::vec3 closetPoint = lineO + projectLength * lineD;

        return glm::length(point - closetPoint);
    }

    void GeomtryEditor::buildTree() {
        if (!currentEdiShape) return;

        ChoiceTree->clear();
        ChoiceTree->setDividefunc(divefunc);
        auto& vertexArray = currentEdiShape->getVertexArrary();
        uint32_t vertexCount = vertexArray->GetVertexBuffers()[0]->GetVertexCount();

        for (uint32_t i=0; i<vertexCount; i++) {
            auto data = std::make_pair<glm::vec3, int>(vertexArray->GetVertexPosition(i), i);
            ChoiceTree->add(data);
        }

    }

    //编辑器命令
    //编辑顶点      
    EditVertexCommand::EditVertexCommand(ShapePtr shape, VertexType typ, uint32_t idx, vec Data, vec oldData_, const std::shared_ptr<OcTree<vertexData, VertexHash, VertexEqual>>& tree): 
        EditShape(shape), index(idx), fixType(typ), data(Data), olddata(oldData_), ChoiceTree(tree)
    {}

    bool EditVertexCommand::execute() {
        if (EditShape == nullptr) return false;
        switch(fixType) {
            case VertexType::Position: {
                if (auto* p = std::get_if<glm::vec3>(&data)) {
                    EditShape->getVertexArrary()->UpdateVertexPosition(index, *p);
                    ChoiceTree->setDividefunc(divefunc);
                    ChoiceTree->remove({glm::vec3{0.0}, index});
                    ChoiceTree->add({*p ,index});
                    return true;
                }
                else return false;
            }
            case VertexType::Color: {
                if (auto* p = std::get_if<glm::vec4>(&data)) {
                    EditShape->getVertexArrary()->UpdateVertexColor(index, *p);
                    return true;
                }
                else return false;
            }
            case VertexType::Normal: {
                if (auto* p = std::get_if<glm::vec3>(&data)) {
                    EditShape->getVertexArrary()->UpdateVertexNormal(index, *p);
                    return true;
                }
                else return false;
            }
            case VertexType::UV: {
                if (auto* p = std::get_if<glm::vec2>(&data)) {
                    EditShape->getVertexArrary()->UpdateVertexUV(index, *p);
                    return true;
                }
                else return false;
            }
        }   
        return false;
    }
    bool EditVertexCommand::cancel() {
        switch(fixType) {
            case VertexType::Position: {
                FS_INFO("get Back Position!");
                auto pos = std::get<glm::vec3>(olddata);
                FS_INFO("Old position: ({}, {}, {})", pos.x, pos.y, pos.z);
                EditShape->getVertexArrary()->UpdateVertexPosition(index, pos);
                ChoiceTree->setDividefunc(divefunc);
                ChoiceTree->remove({glm::vec3{0.0}, index});
                ChoiceTree->add({pos ,index});
                return true;
            }
            case VertexType::Color: {                
                auto color  = std::get<glm::vec4>(olddata);
                EditShape->getVertexArrary()->UpdateVertexColor(index, color);
            }
            case VertexType::Normal: {
                auto normal = std::get<glm::vec3>(olddata);
                EditShape->getVertexArrary()->UpdateVertexNormal(index, normal);
                return true;
            }
            case VertexType::UV: {
                auto uv = std::get<glm::vec2>(olddata);
                EditShape->getVertexArrary()->UpdateVertexUV(index, uv);
                return true;
            }
        }
        return false;
        
    }
}