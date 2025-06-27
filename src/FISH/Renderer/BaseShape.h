#pragma once
#include "FISH/Core.h"

namespace FISH
{
    //��״��
    class API_ Shape {
    public:
        Shape();

        ~Shape();

        //ֱ�Ӵ���VertexBuffer��index
        Shape(std::shared_ptr<VertexBuffer> vertx, std::shared_ptr<IndexBuffer> index);

        //��������
        static Shape* CreateBox(float size);

        //��������
        static Shape* CreateSphere(float radius);
        
        //����ƽ��
        static Shape* CreatePlan(float width, float height);
        //2d������
        static Shape* CreateTriangle2D(float size);
        //2d������
        static Shape* CreateRectangle2D(float width, float height);
        //2dԲ��
        static Shape* CreateCircle2D(float radius, int segments = 32);
        //2d��
        static Shape* CreateLine2D(float x1, float y1, float x2, float y2, float thickness = 1.0f);

        //��ȡ�������
        uint32_t GetIndexCounts() const;

        //ʹ�ø�ģ��vao
        inline void useShape() { 
            mVao->bind();
        }

        //ʹ�ô�ģ�͵�������ͼ
        inline uint64_t useTexture() {
            return mTexture->getHandle();
        }

        inline void unuseShape() {
            mVao->unbind();
        }

        //���ƴ���״
        inline void render(int ElementType) {mVao->renderIndex(0, ElementType);}

        //����ʹ������
        void setTexture(const std::shared_ptr<Texture>& texture);

    private:
        std::unique_ptr<VertexArray>    mVao{nullptr};
        std::shared_ptr<Texture>        mTexture{nullptr};
    };



} // namespace FISH
