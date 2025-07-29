#include "fspcs.h"
#include "FISH/FileManger.h"
#include "FISH/Time.h"
#include "FISH/MouseButtonCodes.h"
#include "FISH/Events/MouseEvent.h"
#include "FISH/Log.h"
#include "FISH/Object/Camera.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/RenderElement.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/Object/Object.h"
#include "FISH/Object/SpotLight.h"
#include "FISH/Object/PointLight.h"
#include "FISH/Object/DirectionLight.h"
#include "FISH/Object/Camera.h"
#include "FISH/Object/Mesh.h"
#include "FISH/Object/SkyBox.h"
#include "FISH/Object/Mesh.h"
#include "FISH/physics/ObjectCast.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <variant>
#include "FISH/Application.h" 
#include "Platform/OpenGL/imgui_impl_glfw.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h" 
#include "FISH/ImGui/ImGuiLayer.h"

#include "ColliderEditor.h"

#include "FISH/physics/RayTest.h"
#include "FISH/input.h"
#include "FISH/Renderer/API.h"
#include "FISH/Renderer/Shader.h"
#include "FISH/Renderer/Renderstatus.h"
#include "FISH/Renderer/Renderer.h"
#include "FISH/Application.h"


namespace FISH {
    std::shared_ptr<Renderstatus> renderStatus = nullptr;
    ColliderEditor* ColliderEditor::mPtr{nullptr};


    void ColliderEditor::OnAttach() {

        if (renderStatus == nullptr) {
            renderStatus.reset(Renderstatus::CreateRenderstatus());
        }
    }

    void ColliderEditor::OnDetach() {
        //退出时保存

    }
    
    void ColliderEditor::OnUpdate(float dt) {
        if (enableTag == 0) return;
        Renderer::setUseCamera(mViewer);
        //保存快捷键
        if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_S))
            needSave = 1;
        //删除选中
        if (Input::IsKeyPressedOnce(FS_KEY_DELETE) && !BeenChoices.empty()) 
            for (auto& [box, _] : BeenChoices) erase(box);
        
        //全选
        if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_A) && Mode == Modify) {
            for (auto& box : GameBoxs) {
                BeenChoices.insert({box, std::pair{Json(), false}});
                box->getMesh()->allowHightLight(1);
            }
        }
        //撤销
        if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_Z)) undo();
        //重做
        if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_Y)) redo();
        //复制
        if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_C)) Copy();
        //粘贴
        if (Input::IsKeyPressed(FS_KEY_LEFT_CONTROL) && Input::IsKeyPressedOnce(FS_KEY_V)) Paste();
        //相机锁
        if (Input::IsKeyPressed(FS_KEY_LEFT_ALT) && Input::IsKeyPressedOnce(FS_KEY_L)) {
            FreeControl = !FreeControl;
            if (FreeControl) APP.LockCursor();
            mViewer->setAllowedControl(FreeControl);
            if (!FreeControl) APP.UnLockCursor();
        }

        if (Mode == View && !FreeControl) {
            if (Input::IsKeyPressed(FS_KEY_A)) mViewer->setPosition(mViewer->getPosition() + glm::normalize(mViewer->getRight())*MoveSpeed*dt);
            if (Input::IsKeyPressed(FS_KEY_D)) mViewer->setPosition(mViewer->getPosition() - glm::normalize(mViewer->getRight())*MoveSpeed*dt);
            if (Input::IsKeyPressed(FS_KEY_SPACE)) mViewer->setPosition(mViewer->getPosition() + glm::vec3(0.0, MoveSpeed*dt, 0.0));
            if (Input::IsKeyPressed(FS_KEY_LEFT_SHIFT)) mViewer->setPosition(mViewer->getPosition() + glm::vec3(0.0, -MoveSpeed*dt, 0.0));
        }
        
        auto currentProj = mViewer->getProjectMatrix();
        auto currentView = mViewer->getViewMatrix();
        
        renderStatus->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);
        renderStatus->setstatusFunc(SetType::StencilFunc, FuncType::Always, 1, 0xff);
        renderStatus->setstatusFunc(FISH::SetType::StencilOperation, FISH::FuncType::Replace, FISH::FuncType::Replace, FISH::FuncType::Replace);
        if (DraggingTag && currentAABB) Renderer::renderColliderBox(currentAABB, {1.0, 0, 0});
        renderStatus->setstatusFunc(FISH::SetType::DepthFunc, FISH::FuncType::DepthLess);
        renderStatus->setstatusFunc(FISH::SetType::StencilOperation, FISH::FuncType::Keep, FISH::FuncType::Keep, FISH::FuncType::Keep);
        renderStatus->setstatusFunc(FISH::SetType::StencilMask, FISH::FuncType::ZERO, 0xff);
        
        for (auto& box : Colliders) Renderer::renderColliderBox(box, {0.0, 0.0, 1.0});

        if (Mode == Modify && Input::IsMouseButtonPressedOnce(FS_MOUSE_BUTTON_LEFT)) {
            bool HadChoice = 0;
            for (auto& box : GameBoxs) {
                if (
                    !BeenChoices.contains(box) &&
                    RayTest::IsRayCastGameObj(
                        FISH::MousePosToWorldPos_InViewPlane(
                            FISH::Input::GetMousePos(),
                            mViewer,
                            APP.GetWindow().GetWidth(),
                            APP.GetWindow().GetHeight()
                        ),
                        FISH::getRayDirectionFromMouse(
                            FISH::Input::GetMousePos(),
                            mViewer,
                            APP.GetWindow().GetWidth(),
                            APP.GetWindow().GetHeight()
                        ),
                        box
                    )
                ) {
                    if (!Input::IsKeyPressed(FS_KEY_LEFT_CONTROL)) {    
                        for (auto& [preBox, _] : BeenChoices) {
                            preBox->getMesh()->allowHightLight(0);
                        }
                        BeenChoices.clear();
                    }
                    box->getMesh()->allowHightLight(1);
                    BeenChoices.insert({box, std::pair{Json(), 0}});
                    HadChoice = 1;
                    break;
                }
            }
            if (!HadChoice) {
                for (auto& [preBox, _] : BeenChoices) {
                    preBox->getMesh()->allowHightLight(0);
                }
                BeenChoices.clear();
            } 
        }
    }

    void ColliderEditor::OnImGuiRender() {
        if (enableTag == 0) return;

        if (needSave) {
            if (currentFileName.empty()) {
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 
                                         ImGui::GetIO().DisplaySize.y * 0.5f), 
                                   ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                ImGui::Begin(" ");
                ImGui::Text("Enter a Filename!");
                ImGui::Spacing();

                string setname;
                if (ImGui::InputText("##filename", &setname, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    currentFileName = setname;
                    saveToFile(currentFileName);
                }
                if (ImGui::Button("Cancel")) needSave = 0;
                ImGui::End();
            }
            else {
                needSave = 0;
                saveToFile(currentFileName);
            }
        }

        //基本设置

        ImGui::Begin("AABB Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("Load Json:");
        string FileName;
        if(ImGui::InputText("##LoadJson", &FileName, ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (!FileName.empty()) {
                saveToFile(currentFileName);
                currentFileName = FileName;
                if (!loadFromFile(currentFileName)) {
                    ImGui::Text("Error File Name");
                    FS_ERROR("Can not Load Json File: {0}", currentFileName);
                }
                else {
                    FS_INFO("Done! load {0} into Editor!", currentFileName);
                }
            }
        }
        ImGui::Spacing();

        ImGui::Text("Creation Settings");
        ImGui::SliderFloat("Creation Distance", &distance, 0.1f, 100, "%.1f");
        ImGui::SliderFloat("Drag Speed Setting", &DragSpeed, 0.1, 100, "%.2f");
        ImGui::SliderFloat("Viewer Speed Setting", &MoveSpeed, 0.1, 100, "%.2f");
        string CamreaMode = FreeControl ? "Free" : "Lock";
        if (ImGui::Button(CamreaMode.c_str())) {
            FreeControl = !FreeControl;
            if (FreeControl) APP.LockCursor();
            mViewer->setAllowedControl(FreeControl);
            if (!FreeControl) APP.UnLockCursor();
        }


        int currentType = (int)(mType);
        //选择创建的模型
        ImGui::Text("Mesh Draw Mode:");
        const char* types[] = {"None", "Box", "Plan", "Sphere"};
        if (ImGui::Combo("##Mesh Type", &currentType, types, IM_ARRAYSIZE(types))) {
            mType = (ShapeType)(currentType);
        }

        //选择模式
        ImGui::Text("ColliderEditor Mode:");
        int currentMode = (int)(Mode);
        const char* Modes[] = {"View", "Draw", "Modify"};
        if (ImGui::Combo("##Mode", &currentMode, Modes, IM_ARRAYSIZE(Modes))) {
            Mode = (EditMode)(currentMode);
        }

        if (DraggingTag) {
            ImGui::Spacing();
            ImGui::Text("Box Info-");
            ImGui::Text("   zOffset: %.2f", zOffset);
        }
        bool Pos_Modifiction = 0;
        //选中对象信息
        if (!BeenChoices.empty()) {
            int count = 0;
            ImGui::Separator();
            ImGui::Text("Select Objects: %d", BeenChoices.size());
            for (auto& [selectObj, state] : BeenChoices) {
                auto& [file, tag] = state;
                Json preInfo;
                ImGui::Separator();
                string tmp = "SelectObj" + std::to_string(count);
                ImGui::Text(tmp.c_str());
                ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);

                //设置名字
                ImGui::Text("Name:");
                string Name = selectObj->getName();
                if(ImGui::InputText(("##setName" + std::to_string(count)).c_str(), &Name, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    preInfo = serializeBox(selectObj);
                    selectObj->setName(Name);
                    PushModifyCommand(selectObj, preInfo);
                }
                
                //全局位置
                tmp = ("Game Position(" + std::to_string(count) + ")");
                ImGui::Text((tmp + " :").c_str());
                auto pos = selectObj->getPosition();
                if (ImGui::DragFloat3("##BoxPos", &pos.x, 0.01)) {
                    selectObj->setPosition(pos);
                    if (!Pos_Modifiction) Pos_Modifiction = 1;
                }
                if ((ImGui::IsItemActivated() || Pos_Modifiction) && !tag) {
                    FS_INFO("Setting world Position");
                    file = serializeBox(selectObj);
                    tag = 1;
                }
                if (ImGui::IsItemDeactivatedAfterEdit() && tag)  {
                    PushModifyCommand(selectObj, file), tag = 0;
                }
                //////////////////////////////////////////////

                
                //碰撞体位置
                tmp = "Collider Position(" + std::to_string(count) + ")";
                auto Colliderpos = selectObj->getBounds()->getPosition();
                ImGui::Text((tmp + " :").c_str());
                if (ImGui::DragFloat3(("##" + tmp).c_str(), &Colliderpos.x, 0.01)) {
                        selectObj->getBounds()->setPosition(Colliderpos);
                }
                if (ImGui::IsItemActivated() && !tag) {
                    FS_INFO("Setting Collider Position");
                    file = serializeBox(selectObj);
                    tag = 1;
                }
                if (ImGui::IsItemDeactivatedAfterEdit() && tag)  {
                    PushModifyCommand(selectObj, file), tag = 0;
                }
                ////////////////////////////////////////


                //碰撞体大小
                auto aabbPtr = PtrCastTo<AABB>(selectObj->getBounds());
                if (aabbPtr) {
                    glm::vec3 min = aabbPtr->getMin();
                    glm::vec3 max = aabbPtr->getMax();
                    ImGui::Text("AABB Min/Max:");
                    if (ImGui::DragFloat3(("##AABBMin" + std::to_string(count)).c_str(), &min.x, 0.01f)) 
                        aabbPtr->setBounding(min, max);
                    if (ImGui::IsItemActivated() && !tag) {
                        FS_INFO("Setting AABB Min/Max");
                        file = serializeBox(selectObj);
                        tag = 1;
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit() && tag) 
                        PushModifyCommand(selectObj, file), tag = 0;

                    if(ImGui::DragFloat3(("##AABBMax" + std::to_string(count)).c_str(), &max.x, 0.01f))
                        aabbPtr->setBounding(min, max);
                    if (ImGui::IsItemActivated() && !tag) {
                        FS_INFO("Setting AABB Min/Max");
                        file = serializeBox(selectObj);
                        tag = 1;
                    }
                    if (ImGui::IsItemDeactivatedAfterEdit() && tag) 
                        PushModifyCommand(selectObj, file), tag = 0;
                    
                }
                /////////////////////////////////////////////////////////////////////

                //渲染体位置
                tmp = "Mesh Position(" + std::to_string(count) + ")";
                auto Meshpos = selectObj->getMesh()->getPosition();
                ImGui::Text((tmp + " :").c_str());
                if (ImGui::DragFloat3(("##" + tmp).c_str(), &Meshpos.x, 0.01)) {
                    selectObj->getMesh()->setPosition(Meshpos);
                }
                if (ImGui::IsItemActivated() && !tag) {
                    FS_INFO("Setting Mesh Position");
                    file = serializeBox(selectObj);
                    tag = 1;
                }
                if (ImGui::IsItemDeactivatedAfterEdit() && tag)  {
                    PushModifyCommand(selectObj, file), tag = 0;
                }
                /////////////////////////////////

                bool isDrag = ImGui::IsItemActivated();
                
                //渲染体材质
                tmp = "Mesh Texture(" + std::to_string(count) + ")";
                string path = selectObj->getMesh()->getShape()->getTexturePath();
                if (ImGui::InputText(("##" + tmp).c_str(), &path, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    std::replace(path.begin(), path.end(), '\\', '/');
                    Json preInfo = serializeBox(selectObj);
                    std::shared_ptr<Texture> tex; tex = (Texture::CreateTextureFromPath(path));
                    if (tex != nullptr) selectObj->getMesh()->getShape()->setTexture(tex);
                    PushModifyCommand(selectObj, preInfo);
                }
                ImGui::Image(static_cast<ImTextureID>(selectObj->getMesh()->getShape()->getTextureID()), ImVec2(100, 100));


                tmp = "Delete Object(" + std::to_string(count) + ")";
                if (ImGui::Button(tmp.c_str())) {
                    erase(selectObj);
                    BeenChoices.erase(selectObj);
                    break;
                }

                
                count++;

            }
        }
        ImGui::End();
    }

    void ColliderEditor::OnEvent(Event &event) {
        if (enableTag == 0) return;
        EventDispatcher dipatcher(event);
        dipatcher.Dispatch<MouseButtonPressedEvent>(FS_BIND_EVENT_FN(ColliderEditor::MousePressed));
        dipatcher.Dispatch<MouseButtonReleasedEvent>(FS_BIND_EVENT_FN(ColliderEditor::MouseRelesed));
        dipatcher.Dispatch<MouseMovedEvent>(FS_BIND_EVENT_FN(ColliderEditor::MouseMove));
        dipatcher.Dispatch<MouseScrolledEvent>(FS_BIND_EVENT_FN(ColliderEditor::MouseScrolled));

    }
    bool ColliderEditor::MouseScrolled(MouseScrolledEvent &e) {
        if (Mode == Draw && DraggingTag && !needSave) {
            zOffset += e.GetY() * 0.1f;
        }
        else if (Mode == Modify && DraggingTag && !needSave) {
            float deltaZ = e.GetY() * DragSpeed * 0.01;
            for (auto& [box, _] : BeenChoices) {
                glm::vec3 newPos = box->getPosition() + glm::vec3(0, 0, deltaZ);
                box->setPosition(newPos);
            }
        }
        else if (Mode == View) {
            float dz = e.GetY() * MoveSpeed * 0.01;
            mViewer->setPosition(mViewer->getPosition() + mViewer->getFront()*dz);
            distance += -dz;
        } 
        return false;
    }
    bool ColliderEditor::MousePressed(MouseButtonPressedEvent &e)
    {
        if (e.GetMouseBUtton() == FS_MOUSE_BUTTON_LEFT && !DraggingTag && Mode == Draw && !needSave) {
            FS_INFO("build Press");
            startPos = MousePosToWorldPos_AtDistance(
                Input::GetMousePos(),
                mViewer,
                APP.GetWindow().GetWidth(),
                APP.GetWindow().GetHeight(),
                distance
            );  

            if (currentAABB == nullptr) currentAABB = std::make_shared<AABB>(glm::vec3(0), glm::vec3(0));
            currentAABB->setPosition(startPos);
            DraggingTag = 1;
        }
        else if (e.GetMouseBUtton() == FS_MOUSE_BUTTON_RIGHT && !DraggingTag && Mode == Modify && !needSave && !BeenChoices.empty()) {
            FS_INFO("choice Press!");
            startPos = MousePosToWorldPos_InViewPlane(
                Input::GetMousePos(),
                mViewer,
                APP.GetWindow().GetWidth(),
                APP.GetWindow().GetHeight()
            );
            for (auto &[selectObj, info] : BeenChoices) {
                info.first = serializeBox(selectObj);
            }
            APP.LockCursor();

            DraggingTag = 1;
        }
        return false;
    }
    bool ColliderEditor::MouseRelesed(MouseButtonReleasedEvent &e) {
        if (e.GetMouseBUtton() == FS_MOUSE_BUTTON_LEFT && DraggingTag && Mode == Draw && !needSave) {
            auto currentPos = MousePosToWorldPos_AtDistance(
                Input::GetMousePos(),
                mViewer,
                APP.GetWindow().GetWidth(),
                APP.GetWindow().GetHeight(),
                distance
            );  
            
            glm::vec3 minPos = startPos + glm::vec3(
                (glm::min)(0.0f, currentPos.x - startPos.x),
                (glm::min)(0.0f, currentPos.y - startPos.y),
                0
            );
        
            glm::vec3 maxPos = startPos + glm::vec3(
                (glm::max)(0.0f, currentPos.x - startPos.x),
                (glm::max)(0.0f, currentPos.y - startPos.y),
                zOffset
            );
            if (glm::length(maxPos - minPos) < 1.0) return false;

            glm::vec3 center = (minPos + maxPos) * 0.5f;
            auto aabb = std::make_shared<AABB>(minPos, maxPos);
            aabb->setPosition(center);
            glm::vec3 scalevec = aabb->size();

            GameBoxPtr nBox;
            if (mType != ShapeType::Unknown){   
                nBox.reset(new Box(aabb));
                if (mType == ShapeType::Plan) scalevec.z = 0;
                nBox->setRenderMeshFromType(mType, scalevec);

                
                nBox->setPosition(center);

                PushAddCommand(nBox);
                GameBoxs.push_back(nBox);
            }
            Colliders.push_back(aabb);

            currentAABB = nullptr;
            startPos = glm::vec3{0.0};
            DraggingTag = 0;
            zOffset = 1.0f;
        }
        else if (e.GetMouseBUtton() == FS_MOUSE_BUTTON_RIGHT && Mode == Modify && DraggingTag && !needSave) {
            DraggingTag = 0;
            for (auto& [selectObj, info] : BeenChoices) PushModifyCommand(selectObj, info.first);
            APP.UnLockCursor();
        }
        return false;
    }
    bool ColliderEditor::MouseMove(MouseMovedEvent &e) {
        if (DraggingTag && Mode == Draw && !needSave) {
            // if (Input::IsKeyPressed(FS_KEY_EQUAL)) zOffset += 0.01f; // 增加Z轴长度
            // if (Input::IsKeyPressed(FS_KEY_MINUS)) zOffset -= 0.01f; // 减少Z轴长度
            auto currentPos = MousePosToWorldPos_AtDistance(
                Input::GetMousePos(),
                mViewer,
                APP.GetWindow().GetWidth(),
                APP.GetWindow().GetHeight(),
                distance
            );  
            glm::vec3 minOffset = (glm::min)(glm::vec3(0), glm::vec3(currentPos.x - startPos.x, currentPos.y - startPos.y, 0));
            glm::vec3 maxOffset = (glm::max)(glm::vec3(0), glm::vec3(currentPos.x - startPos.x, currentPos.y - startPos.y, zOffset));
            currentAABB->setBounding(minOffset, maxOffset);
        }
        if (DraggingTag && Mode == Modify && !needSave) {
            auto currentPos = MousePosToWorldPos_InViewPlane(
                Input::GetMousePos(),
                mViewer,
                APP.GetWindow().GetWidth(),
                APP.GetWindow().GetHeight()
            );
            auto Dpos = currentPos - startPos;
            Dpos *= DragSpeed;
            startPos = currentPos;
            for (auto& [box, _] : BeenChoices) {
                auto nPos = box->getPosition() + glm::vec3(Dpos.x, Dpos.y, 0.0);
                box->setPosition(nPos);
            }
        }
        return false;
    }

    void ColliderEditor::erase(const GameBoxPtr& box) {
        if (!box) return;
        eraseInternal(box, true);
    }

    Json ColliderEditor::toJson() const {
        Json data;
        data["GameBoxs"] = Json::array();
        for (const auto& box : GameBoxs) {
            data["GameBoxs"].push_back(serializeBox(box));
        }
        return data;
    }

    void ColliderEditor::fromJson(const Json &data) {
        GameBoxs.clear();
        Colliders.clear();
        BeenChoices.clear();

        if (!data.contains("GameBoxs")) return;

        for (const auto& boxinfo : data["GameBoxs"]) {
            //box;
            GameBoxPtr box;
            applyBoxState(box, boxinfo);
            GameBoxs.push_back(box);
        }
    }

    void ColliderEditor::PushAddCommand(const GameBoxPtr &box) {
        RedoStack.clear();
        if (UndoStack.size() > limSize) UndoStack.pop_front();

        UndoStack.emplace_back(AddCommand{
            box,
            box->getBounds(),
            serializeBox(box)
        });
    }

    void ColliderEditor::PushRemoveCommand(const GameBoxPtr &box) {
        RedoStack.clear();

        if (UndoStack.size() > limSize) UndoStack.pop_front();

        UndoStack.emplace_back(RemoveCommand(
            box,
            box->getBounds(),
            serializeBox(box)
        ));
    }

    void ColliderEditor::PushModifyCommand(const GameBoxPtr &box, const Json &preboxinfo) {
        RedoStack.clear();
        if (preboxinfo.empty()) return;
        auto currentinfo = serializeBox(box);
        if (preboxinfo == currentinfo) return;
        if (UndoStack.size() > limSize) UndoStack.pop_front();
        UndoStack.emplace_back(ModifyCommand{
            box,
            preboxinfo,
            currentinfo
        });
        FS_INFO("Add ModifyCommand!");
    }

    Json ColliderEditor::serializeBox(const GameBoxPtr &box) const {
        Json boxinfo;
        boxinfo["name"] = box->getName();
        boxinfo["world_position"] = box->getPosition();
        boxinfo["mesh_position"] = box->getMesh()->getPosition();
        boxinfo["mesh_type"] = (int)box->getMesh()->getShape()->getShapeType();
        switch (box->getBounds()->getType()) {   
            case ColliderType::AABB :{               
                auto boxptr = Static_PtrCastTo<AABB>(box->getBounds());
                boxinfo["bounds"] = {
                    {"type", (int)(ColliderType::AABB)},
                    {"min", boxptr->getMin()},
                    {"max", boxptr->getMax()},
                    {"position", boxptr->getPosition()}
                };
                break;
            }
            //预留..
        }
        boxinfo["Texture_path"] = box->getMesh()->getShape()->getTexturePath();
        return boxinfo;
    }

    void ColliderEditor::applyBoxState(GameBoxPtr &box, const Json &boxinfo) {
        ColliderPtr collider;
        if (box != nullptr) {
            collider = box->getBounds();
            switch ((ColliderType)(boxinfo["bounds"]["type"])) {
                case ColliderType::AABB: {
                    auto ptr = Static_PtrCastTo<AABB>(collider);
                    ptr->setBounding(
                        boxinfo["bounds"]["min"],
                        boxinfo["bounds"]["max"]
                    );
                    collider->setPosition(boxinfo["bounds"]["position"]);
                    Colliders.push_back(Static_PtrCastTo<AABB>(collider));
                    break;
                }
                //预留..
            }
            //直接原地修改
            auto scale = collider->size();
            if ((ShapeType)boxinfo["mesh_type"] == ShapeType::Plan) scale.z = 0.0f;
            box->setRenderMeshFromType((ShapeType)boxinfo["mesh_type"], scale);
            box->setPosition(boxinfo["world_position"]);
            box->getMesh()->setPosition(boxinfo["mesh_position"]);
            box->getBounds()->setPosition(boxinfo["bounds"]["position"]);
            box->setName(boxinfo["name"]);
            
        }
        else {
            //新增
            switch ((ColliderType)(boxinfo["bounds"]["type"])) {
                case ColliderType::AABB: {
                    collider = std::make_shared<AABB>(
                        boxinfo["bounds"]["min"],
                        boxinfo["bounds"]["max"]
                    );
                    collider->setPosition(boxinfo["bounds"]["position"]);
                    Colliders.push_back(Static_PtrCastTo<AABB>(collider));
                    break;
                }
                //预留..
            }
            box.reset(new Box(Static_PtrCastTo<AABB>(collider)));
            auto scale = collider->size();
            if ((ShapeType)boxinfo["mesh_type"] == ShapeType::Plan) scale.z = 0.0f;
            box->setRenderMeshFromType((ShapeType)boxinfo["mesh_type"], scale);
            box->setPosition(boxinfo["world_position"]);
            box->getMesh()->setPosition(boxinfo["mesh_position"]);
            box->getBounds()->setPosition(boxinfo["bounds"]["position"]);
            box->setName(boxinfo["name"]);
        }


        if (boxinfo.contains("Texture_path") && boxinfo["Texture_path"] != box->getMesh()->getShape()->getTexturePath()) {
            string path = boxinfo["Texture_path"];
            std::replace(path.begin(), path.end(), '\\', '/');
            box->getMesh()->getShape()->setTexture(Texture::CreateTextureFromPath(path));
        }

    }

    void ColliderEditor::eraseInternal(const GameBoxPtr &box, bool recordUndo) {
        if (recordUndo) {
            PushRemoveCommand(box);
        }
        
        auto it = std::find(GameBoxs.begin(), GameBoxs.end(), box);
        if (it != GameBoxs.end()) {
            auto bounds = (*it)->getBounds();
            GameBoxs.erase(it);
            
            Colliders.erase(std::remove_if(Colliders.begin(), Colliders.end(),
                [&bounds](const AABBPtr& collider) {
                    return collider == bounds;
                }), Colliders.end());
        }
        
        BeenChoices.erase(box);
    
    }

    void ColliderEditor::addBoxInternal(GameBoxPtr &box, const Json &state, bool recordUndo) {
        applyBoxState(box, state);
        GameBoxs.push_back(box);
        Colliders.push_back(Static_PtrCastTo<AABB>(box->getBounds()));
        
        if (recordUndo) PushAddCommand(box);
    }

    void ColliderEditor::undo() {
        if (UndoStack.empty()) return;
        
        EditCommand cmd = std::move(UndoStack.back());
        UndoStack.pop_back();

        std::visit(overload{
            [this](AddCommand& add) {
                eraseInternal(add.box, false);
                RedoStack.emplace_back(RemoveCommand{
                    add.box,
                    add.collider,
                    add.initialState
                });
                FS_INFO("UnDO AddCommand");
            },

            [this](RemoveCommand& remove) {
                addBoxInternal(remove.box, remove.previousState, false);
                RedoStack.emplace_back(AddCommand(
                    remove.box,
                    remove.collider,
                    remove.previousState
                ));
                FS_INFO("UnDO RemoveCommand");

            },

            [this](ModifyCommand& modify) {
                auto currentState = serializeBox(modify.box);
                applyBoxState(modify.box, modify.previousState);
                RedoStack.push_back(ModifyCommand(
                    modify.box,
                    currentState,
                    modify.previousState
                ));
                FS_INFO("UnDO ModifyCommand");

            }
        }, cmd);
    }

    void ColliderEditor::redo() {
        if (RedoStack.empty()) return;

        EditCommand cmd =std::move(RedoStack.back());
        RedoStack.pop_back();

        std::visit(overload{
            [this](AddCommand& add) {
                eraseInternal(add.box, false);
                UndoStack.emplace_back(RemoveCommand{
                    add.box,
                    add.collider,
                    add.initialState
                });
                FS_INFO("Redo AddCommand!");

            },

            [this](RemoveCommand& remove) {
                addBoxInternal(remove.box, remove.previousState, false);
                UndoStack.emplace_back(AddCommand(
                    remove.box,
                    remove.collider,
                    remove.previousState
                ));
                FS_INFO("Redo RemoveCommand!");

            },

            [this](ModifyCommand& modify) {
                applyBoxState(modify.box, modify.previousState);
                UndoStack.push_back(ModifyCommand(
                    modify.box,
                    modify.previousState,
                    modify.newState
                ));
                FS_INFO("Redo ModifyCommand!");
            }
        }, cmd);

    }

    void ColliderEditor::Copy() {
        if (BeenChoices.empty() && Mode != Modify) return;
        mClipBoard = Json::array();
        for (const auto& [box, _] : BeenChoices) mClipBoard.push_back(serializeBox(box));
        FS_INFO("Copy {0} Objects", BeenChoices.size());
    }

    void ColliderEditor::Paste() {
        if (mClipBoard.empty()) return;

        for (auto& [box, _] : BeenChoices) box->getMesh()->allowHightLight(0);
        BeenChoices.clear();

        for (const auto& info : mClipBoard) {
            GameBoxPtr nBox;
            applyBoxState(nBox, info);
            nBox->setPosition(nBox->getPosition() + glm::vec3(1.0, 1.0, 0.0));

            GameBoxs.push_back(nBox);
            Colliders.push_back(Static_PtrCastTo<AABB>(nBox->getBounds()));
            BeenChoices.insert({nBox, {Json(), false}});
            nBox->getMesh()->allowHightLight(1);
            PushAddCommand(nBox);
        }
        FS_INFO("Pasted {0} Objects", BeenChoices.size());
    }

    bool ColliderEditor::saveToFile(const string &filename) {
        JsonFileStorage storage("GameBoxData");
        return storage.save(filename, toJson());
    }

    bool ColliderEditor::loadFromFile(const string &filename) {
        JsonFileStorage storage("GameBoxData");
        Json data;
        if (storage.load(filename, data)) {
            fromJson(data);
            return true;
        }
        return false;
    }

    ColliderEditor *ColliderEditor::get()
    {
        if (mPtr == nullptr) {
            FS_INFO("Create ColliderEditor..");
            mPtr = new ColliderEditor();
        }
        return mPtr;
    }
}
