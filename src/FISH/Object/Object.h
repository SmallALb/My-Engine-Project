#pragma once

#include "FISH/Core.h"



namespace FISH {
    //直接设置类型
#define OBJ_CLASS_TYPE(typ)   static ObjType GetStaticType() {return ObjType::typ; }\
                                virtual ObjType GetObjType() const override {return GetStaticType(); }

    //渲染物体类型
    enum class ObjType {
        //3D
        None,
        Camera,
        Light,
        Arrow,
        Mesh,
        Scene,
        SkyBox,
        //2D
        Botton,

    };

    //渲染物体类(3D)
    class API_ Object3D : public std::enable_shared_from_this<Object3D>{
    public:
        Object3D(const std::string& name = "Object3D");
        virtual ~Object3D() {};
        //设置对象位置
        virtual void setPosition(const glm::vec3& pos);

        //旋转对象X角度
        virtual void rotateX(float angle) { mAngleX += angle; }
        //旋转对象Y角度
        virtual void rotateY(float angle) { mAngleY += angle; }
        //旋转对象Z角度
        virtual void rotateZ(float angle) { mAngleZ += angle; }

        //设置对象的X角度
        virtual void setAngleX(float angle) { mAngleX = angle; }
        //设置对象的Y角度
        virtual void setAngleY(float angle) { mAngleY = angle; }
        //设置对象的Z角度
        virtual void setAngleZ(float angle) { mAngleZ = angle; }

        //获取对象的X角度
        virtual float getAngleX() { return mAngleX; }
        //获取对象的Y角度
        virtual float getAngleY() { return  mAngleY; }
        //获取对象的Z角度
        virtual float getAngleZ() { return mAngleZ; }
        //获取名字
        virtual string getName() const {return mName;}
        //获取位置
        virtual glm::vec3 getPosition() const;

        //获取M矩阵（对象的位置旋转信息矩阵）
        virtual	glm::mat4 getModelMatrix() const;
        
        //获取法线矩阵
        virtual glm::mat4 getNormalMatrix() const;

        //缩放物体
        virtual void setScale(const glm::vec3& scale) { mScale = scale; }

        //获取物体缩放信息
        virtual glm::vec3 getScale() const {return mScale;}

        //添加孩子节点
        virtual void addChild(const std::shared_ptr<Object3D>& obj);
        //删除节点
        virtual void eraseChild(const std::shared_ptr<Object3D>& obj);
    
        //获取所有的孩子节点
        virtual std::vector<std::shared_ptr<Object3D>>& getChilds() { return mChilds; };

        //获取父节点
        virtual std::shared_ptr<Object3D> getParent() {return mParent.lock(); }

        // //获取类型
        // virtual ObjType getType() const {return mType;}
        
        //获取类型
        virtual ObjType GetObjType() const = 0;

    protected:
        glm::vec3 mPosition {0.0f};
        float mAngleX{0.0f};
        float mAngleY{0.0f};
        float mAngleZ{0.0f};
        glm::vec3 mScale {1.0f};
        std::weak_ptr<Object3D> mParent;
        std::vector<std::shared_ptr<Object3D>> mChilds{};
        ObjType mType{ObjType::None};
        std::string mName{};
    private:
    };

    //渲染物体类(2D)
    class API_ Object2D : public std::enable_shared_from_this<Object2D>{
    public:
        Object2D(const std::string& Name = "Object2D");
        ~Object2D() {};
        //获取模型矩阵
        virtual	glm::mat3 getModelMatrix() const;
        //获取位置
        virtual glm::vec2 getPosition() const;
        //设置对象位置
        virtual void setPosition(const glm::vec2& pos);
        //旋转对象X方向
        virtual void rotate(float angle) { mAngle += angle; }
        //设置缩放
        virtual void setScale(const glm::vec2& scale) { mScale = scale; }
        //添加孩子节点
        virtual void addChild(const std::shared_ptr<Object2D>& obj);
        //获取所有的孩子节点
        virtual std::vector<std::shared_ptr<Object2D>>& getChilds() { return mChilds; };
        //获取父节点
        virtual std::shared_ptr<Object2D> getParent() {return mParent.lock(); }
        //获取类型
        virtual ObjType GetObjType() const = 0;
    protected :
        glm::vec2 mPosition {0.0f};
        float mAngle{0.0f};
        glm::vec2 mScale {1.0f};
        std::weak_ptr<Object2D> mParent;
        std::vector<std::shared_ptr<Object2D>> mChilds{};
        ObjType mType{ObjType::None};
        std::string mName{};
    };

    using Object3DPtr = std::shared_ptr<Object3D>;
    
} // namespace FISH
