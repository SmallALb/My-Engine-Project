#pragma once
#include "FISH/Core.h"

namespace FISH {
    //形状枚举类
    enum class ShapeType {
        Unknown = 0,
        Box,
        Plan,
        Sphere,
        Circle2D,
        Line2D,
        Triangle2D
    };

    //形状类
    class API_ Shape {
    public:
        Shape();

        ~Shape();

        //直接传入VertexBuffer和index
        Shape(std::shared_ptr<VertexBuffer> vertx, std::shared_ptr<IndexBuffer> index);

        //创建盒体
        static Shape* CreateBox(float size, int subdivisions = 2);

        //创建球体
        static Shape* CreateSphere(float radius, int subdivisions = 32);
        
        //创建平面
        static Shape* CreatePlan(float width, float height);
        //创建矩形
        static Shape* CreateRectangle(float width, float height);

        //2d三角形
        static Shape* CreateTriangle2D(float size);
        //2d长方形
        static Shape* CreateRectangle2D(float width, float height);
        //2d圆形
        static Shape* CreateCircle2D(float radius, int segments = 32);
        //2d线
        static Shape* CreateLine2D(float x1, float y1, float x2, float y2, float thickness = 1.0f);

        //获取坐标计数
        uint32_t GetIndexCounts() const;

        //使用该模型vao
        inline void useShape() { 
            mVao->bind();
        }

        inline void unuseShape() {
            mVao->unbind();
        }

        //绘制此形状
        inline void render(int ElementType) {
            useShape();
            mVao->renderIndex(0, ElementType);
            unuseShape();
        }

        inline ShapeType getShapeType() const {return mType;}

        std::unique_ptr<VertexArray>& getVertexArrary();

    private:
        std::unique_ptr<VertexArray>    mVao{nullptr};
        ShapeType                       mType{ShapeType::Unknown};
    };

    using ShapePtr = std::shared_ptr<Shape>;


} // namespace FISH
