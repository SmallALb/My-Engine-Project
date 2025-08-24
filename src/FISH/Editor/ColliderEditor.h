#pragma once
#include "FISH/Game/GameBox.h"
#include "FISH/Layer.h"
//#define DEBUG
namespace FISH {
    class ColliderEditor : public Layer {
        ColliderEditor(bool NoReadIni = 1) {
            Colliders.reserve(100);
            GameBoxs.reserve(100);
            
                BeenChoices.reserve(100);
                IniFileStorage iniManager;
                Ini config;
                // 设置默认值
                distance = 10.0f;
                Mode = View;
                mType = ShapeType::Box;
                DragSpeed = 10.0f;
                currentFileName = "";
                MoveSpeed = 1.0f;
                
                if (NoReadIni) return;
                // 尝试加载配置
                if (iniManager.load("ColliderEditorInfo", config)) {
                    // 成功加载则从配置文件读取
                    if (config["Info"].contains("Distance")) 
                        distance = std::stof(config["Info"]["Distance"]);
                    if (config["Info"].contains("Mode")) 
                        Mode = (EditMode)std::stoi(config["Info"]["Mode"]);
                    if (config["Info"].contains("DrawType")) 
                        mType = (ShapeType)std::stoi(config["Info"]["DrawType"]);
                    if (config["Info"].contains("OpenFileName")) 
                        currentFileName = config["Info"]["OpenFileName"];
                    if (config["Info"].contains("DragSpeed"))
                        DragSpeed = std::stof(config["Info"]["DragSpeed"]);
                    if (config["Info"].contains("MoveSpeed"))
                        MoveSpeed = std::stof(config["Info"]["MoveSpeed"]);
                        
                }
                
                // 保存配置（无论是否加载成功都保存，确保文件存在）
                config["Info"]["Distance"] = std::to_string(distance);
                config["Info"]["Mode"] = std::to_string(Mode);
                config["Info"]["DrawType"] = std::to_string((int)mType);
                config["Info"]["OpenFileName"] = currentFileName;
                config["Info"]["DragSpeed"] = std::to_string(DragSpeed);
                config["Info"]["MoveSpeed"] = std::to_string(MoveSpeed);
                iniManager.save("ColliderEditorInfo", config);  // 使用一致的文件名
                if (!currentFileName.empty()) loadFromFile(currentFileName);
            
        }
    public:
        ~ColliderEditor() {
            
                saveToFile(currentFileName);
                IniFileStorage iniManager;
                Ini config;
                config["Info"]["Distance"] = std::to_string(distance);
                config["Info"]["Mode"] = std::to_string(Mode);
                config["Info"]["DrawType"] = std::to_string((int)mType);
                config["Info"]["OpenFileName"] = currentFileName;
                config["Info"]["DragSpeed"] = std::to_string(DragSpeed);
                config["Info"]["MoveSpeed"] = std::to_string(MoveSpeed);
                iniManager.save("ColliderEditorInfo", config);
            
        }

        void OnAttach() override;
        //销毁
        void OnDetach() override;
        //更新
        void OnUpdate(float dt) override;
        //渲染UI
        void OnImGuiRender() override;
        //返回生成盒体的数据
        inline const std::vector<GameBoxPtr>& outputGameBoxData() const {return GameBoxs;}
        //加载到文件中
        bool loadFromFile(const string& filename);
        //事件
        
        void OnEvent(Event& event) override;
        //设置观察者（相机）接下来的创建都会在这个相机中进行
        inline void setViewer(const CameraPtr& viewer) {
            mViewer = viewer;
            if (mViewer->getCameraType() == CameraType::PERSPECTIVE)
                Static_PtrCastTo<perspectiveCamera>(mViewer)->setSpeed(MoveSpeed*0.01);
            mViewer->setPosition({0.0, 0.0, -13.0});
            mViewer->setLookAt({0.0, 0.0, 0.0});
        }
        //设置距离
        inline void setDistance(float dis) {distance = dis;}

        
        //设置生成模型类型
        inline void setMeshType(ShapeType typ) {mType = typ;}
        //保存数据
        bool saveToFile(const string& filename);
        
        //单例
        static ColliderEditor* mPtr;
        static ColliderEditor* get();
        
        void openColliderEditor() { enableTag = 1; }
        void closeColliderEditor(){ enableTag = 0; }
        
    private:
        
        bool MouseScrolled(MouseScrolledEvent& e);
        bool MousePressed(MouseButtonPressedEvent& e);
        bool MouseRelesed(MouseButtonReleasedEvent& e);
        bool MouseMove(MouseMovedEvent& e);
        void erase(const GameBoxPtr& box);

        //信息写入Json
        Json toJson() const;
        
        Json serializeBox(const GameBoxPtr& box) const;
        
        //从Json中读取
        void fromJson(const Json& data);
        void applyBoxState(GameBoxPtr& box, const Json& state);

    private:
        std::vector<AABBPtr>    Colliders;  
        std::vector<GameBoxPtr> GameBoxs;
        
        AABBPtr                 currentAABB{nullptr};
        glm::vec3               startPos{0, 0, 0};
        bool                    DraggingTag{0};
        CameraPtr               mViewer;
        //被选中的对象
        std::unordered_map<GameBoxPtr, std::pair<Json, bool>> BeenChoices;
        //在当前平面的创建距离
        float                   distance{10.0};
        bool                    closeUI{0};
        bool                    enableTag{0};
        bool                    needSave{0};
        bool                    FreeControl{0};
        bool                    newFileTag{0};
        float                   zOffset{1.0};
        float                   DragSpeed{10.0f};
        float                   MoveSpeed{1.0};
        glm::vec3               lastCameraPos;
        //生成模型Type
        ShapeType               mType{ShapeType::Box};
        string                  currentFileName{};
        TextureHandleType       Texturetype{TextureHandleType::None};
        //当前选择的数据
        string                  currentTexturepath{""};
        string                  currentAniname{""};
        int                     currentAnibeginidx{-1};
        int                     currentAnisize{-1};
        int                     currentAniduration{-1};
        //撤销
    private:
        //编辑器指令(用于撤销)
        struct AddCommand {
            GameBoxPtr box;
            ColliderPtr    collider;
            Json initialState;
        };

        struct RemoveCommand {
            GameBoxPtr box;
            ColliderPtr    collider;
            Json previousState;
        };

        struct ModifyCommand {
            GameBoxPtr box;
            Json previousState;
            Json newState;
        };

        using EditCommand = std::variant<AddCommand, RemoveCommand, ModifyCommand>;

        //指令栈
        std::deque<EditCommand> UndoStack{};
        std::deque<EditCommand> RedoStack{};
        const size_t limSize{50};
        
        void PushAddCommand(const GameBoxPtr& box);
        void PushRemoveCommand(const GameBoxPtr& box);
        void PushModifyCommand(const GameBoxPtr& box, const Json& preboxinfo);

        void eraseInternal(const GameBoxPtr& box, bool recordUndo);
        void addBoxInternal(GameBoxPtr& box, const Json& state, bool recordUndo);
        //撤销
        void undo();
        //撤销当前撤销操作
        void redo();
    //控制模式
    private:
        enum EditMode {
            View,
            Draw,
            Modify
        };

        EditMode Mode{View};
    //复制粘贴
    private:
        Json mClipBoard;
        //复制
        void Copy();
        //粘贴
        void Paste(); 
        
    };


}