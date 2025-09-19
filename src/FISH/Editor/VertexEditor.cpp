#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/Renderer/Renderstatus.h"

#include "VertexEditor.h"
#include "FISH/ImGui/ImGuiLayer.h"

#include "FISH/Renderer/Shader.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/Renderer/Material.h"
#include "FISH/Object/Object.h"
#include "FISH/Object/SpotLight.h"
#include "FISH/Object/PointLight.h"
#include "FISH/Object/DirectionLight.h"
#include "FISH/Object/Camera.h"
#include "FISH/Object/Mesh.h"
#include "FISH/Object/SkyBox.h"
#include "FISH/physics/ObjectCast.h"
#include "FISH/Renderer/Renderer.h"

namespace FISH {

    GeomtryEditor* GeomtryEditor::mInstance = nullptr;

    GeomtryEditor::~GeomtryEditor() {
    }

    void GeomtryEditor::OnImGuiRender() {
        UI::Begin("Editor");
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("Viewer");
        ImGui::Image(
            mFramebuffer->GetColorAttachmentID(), 
            viewportSize, 
            ImVec2(0, 1),  // UV坐标：左下角
            ImVec2(1, 0)   // UV坐标：右上角
        );
        ImGui::EndChild();

         if (ImGui::CollapsingHeader("Vertex Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& vertexArray = currentEdiShape->getVertexArrary();
                uint32_t vertexCount = vertexArray->GetVertexBuffers()[0]->GetVertexCount();
                
                // 顶点选择器
                static int selectedVertex = 0;
                ImGui::SliderInt("Vertex Index", &selectedVertex, 0, vertexCount - 1);
                
                if (selectedVertex >= 0 && selectedVertex < vertexCount) {
                    // 获取当前顶点数据
                    glm::vec3 position = vertexArray->GetVertexPosition(selectedVertex);
                    glm::vec4 color = vertexArray->GetVertexColor(selectedVertex);
                    glm::vec3 normal = vertexArray->GetVertexNormal(selectedVertex);
                    glm::vec2 uv = vertexArray->GetVertexUV(selectedVertex);
                    
                    // 保存原始数据用于比较
                    static glm::vec3 oldPosition = position;
                    static glm::vec4 oldColor = color;
                    static glm::vec3 oldNormal = normal;
                    static glm::vec2 oldUV = uv;
                    
                    // 位置编辑
                    bool positionEdited = ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f);
                    
                    // 颜色编辑
                    bool colorEdited = ImGui::ColorEdit4("Color", glm::value_ptr(color));
                    
                    // 法线编辑
                    bool normalEdited = ImGui::DragFloat3("Normal", glm::value_ptr(normal), 0.01f, -1.0f, 1.0f);
                    
                    // UV编辑
                    bool uvEdited = ImGui::DragFloat2("UV", glm::value_ptr(uv), 0.01f, 0.0f, 1.0f);
                    
                    // 实时更新显示（但不提交命令）
                    if (positionEdited) {
                        vertexArray->UpdateVertexPosition(selectedVertex, position);
                    }
                    if (colorEdited) {
                        vertexArray->UpdateVertexColor(selectedVertex, color);
                    }
                    if (normalEdited) {
                        vertexArray->UpdateVertexNormal(selectedVertex, normal);
                    }
                    if (uvEdited) {
                        vertexArray->UpdateVertexUV(selectedVertex, uv);
                    }
                    
                    // 检测编辑完成并提交命令
                    if (ImGui::IsItemDeactivatedAfterEdit()) {
                        // 检查哪个属性被修改并创建相应的命令
                        if (position != oldPosition) {
                            auto command = std::make_shared<EditVertexCommand>(
                                currentEdiShape, 
                                VertexType::Position, 
                                selectedVertex, 
                                position
                            );
                            Editor::mEditorCommands.PushCommand(command);
                            oldPosition = position;
                        }
                        
                        if (color != oldColor) {
                            auto command = std::make_shared<EditVertexCommand>(
                                currentEdiShape, 
                                VertexType::Color, 
                                selectedVertex, 
                                color
                            );
                            Editor::mEditorCommands.PushCommand(command);
                            oldColor = color;
                        }
                        
                        if (normal != oldNormal) {
                            auto command = std::make_shared<EditVertexCommand>(
                                currentEdiShape, 
                                VertexType::Normal, 
                                selectedVertex, 
                                normal
                            );
                            Editor::mEditorCommands.PushCommand(command);
                            oldNormal = normal;
                        }
                        
                        if (uv != oldUV) {
                            auto command = std::make_shared<EditVertexCommand>(
                                currentEdiShape, 
                                VertexType::UV, 
                                selectedVertex, 
                                uv
                            );
                            Editor::mEditorCommands.PushCommand(command);
                            oldUV = uv;
                        }
                    }
                    
                    // 如果切换到其他顶点，更新保存的旧数据
                    static int lastSelectedVertex = -1;
                    if (selectedVertex != lastSelectedVertex) {
                        oldPosition = position;
                        oldColor = color;
                        oldNormal = normal;
                        oldUV = uv;
                        lastSelectedVertex = selectedVertex;
                    }
                }
                
                // 批量操作按钮
                
            }
        UI::End();
    }

    void GeomtryEditor::OnAttach() {
        currentEdiShape.reset(FISH::Shape::CreateSphere(1.0));
        mFramebuffer.reset(FrameBuffer::Create(2000, 1700));
        mStatus.reset(Renderstatus::CreateRenderstatus());

        mStatus->enablestatus(StatusType::CleanColor);
        mStatus->enablestatus(StatusType::CleanDepth);


    }

    void GeomtryEditor::OnUpdate(float dt) {
        mFramebuffer->bind();
        mFramebuffer->updateCleanStatus(mStatus->getCleanstatuss());
        Renderer::RenderShapeEdge(currentEdiShape, {0.0, 0.0, 1.0});
        mFramebuffer->unbind();
        
    }

    GeomtryEditor* GeomtryEditor::get() {
        if (mInstance == nullptr) mInstance = new GeomtryEditor();
        return mInstance;
    }

    void GeomtryEditor::addShape() {
        
    }

    void GeomtryEditor::fixVertex() {
    }

    void GeomtryEditor::delShape() {

    }

    //编辑器命令
    //编辑顶点      
    EditVertexCommand::EditVertexCommand(ShapePtr shape, VertexType typ, uint32_t idx, vec Data): 
        EditShape(shape), index(idx), fixType(typ), data(Data)
    {}

    bool EditVertexCommand::execute() {
        if (EditShape == nullptr) return false;
        switch(fixType) {
            case VertexType::Position: {
                if (auto* p = std::get_if<glm::vec3>(&data)) {
                    auto olddata = EditShape->getVertexArrary()->GetVertexPosition(index);
                    EditShape->getVertexArrary()->UpdateVertexPosition(index, *p);
                    data = olddata;
                    return true;
                }
                else return false;
            }
            case VertexType::Color: {
                if (auto* p = std::get_if<glm::vec4>(&data)) {
                    auto olddata = EditShape->getVertexArrary()->GetVertexColor(index);
                    EditShape->getVertexArrary()->UpdateVertexColor(index, *p);
                    data = olddata;
                    return true;
                }
                else return false;
            }
            case VertexType::Normal: {
                if (auto* p = std::get_if<glm::vec3>(&data)) {
                    auto olddata = EditShape->getVertexArrary()->GetVertexNormal(index);
                    EditShape->getVertexArrary()->UpdateVertexNormal(index, *p);
                    data = olddata;
                    return true;
                }
                else return false;
            }
            case VertexType::UV: {
                if (auto* p = std::get_if<glm::vec2>(&data)) {
                    auto olddata = EditShape->getVertexArrary()->GetVertexUV(index);
                                        EditShape->getVertexArrary()->UpdateVertexUV(index, *p);
                    data = olddata;
                    return true;
                }
                else return false;

            }
        }   
        return false;
    }
    bool EditVertexCommand::cancel() {
        return false;
    }

    //添加Shape命令
    AddShapeCommand::AddShapeCommand(std::vector<ShapePtr>& ref_, const ShapePtr& ptr_) : ref(ref_), ptr(ptr_) {}

    bool AddShapeCommand::execute() {
        ref.push_back(ptr);
        return true;
    }

    bool AddShapeCommand::cancel() {
        auto it = std::find(ref.begin(), ref.end(), ptr);
        if (it != ref.end()) {
            std::iter_swap(it, ref.end() - 1);
            ref.pop_back();
        }
        return true;
    }
}