#pragma once

#include "FISH/Core.h"
#include "Camera.h"

namespace FISH {
    class OrthographicCamera : public Camera {
    public:    
        OrthographicCamera(const string& name);
                //设置视角缩放
        virtual void scale(float deltaScale) override;

        //获取看向方向
        virtual glm::vec3 getFront() const override;

        virtual void update() override;

         virtual glm::mat4 getProjectMatrix() override;

        void init(float l, float r, float t, float b, float n, float f);
    private:
        float mLeft = 0.0f;
        float mRight = 0.0f;
        float mTop = 0.0f;
        float mBottom = 0.0f;
    };


}

