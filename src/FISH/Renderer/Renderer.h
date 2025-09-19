#pragma once

#include "FISH/Core.h"

namespace FISH {

    class API_ Renderer {
    public:
        //设置使用的相机
        inline static void setUseCamera(const std::shared_ptr<Camera>& camera) { UseCamera = camera; } 
        
        //设置使用的着色器
        //inline static void setUseShader(const std::shared_ptr<Shader>& shader) { UseShader = shader; }

        //设置环境光照
        inline static void setUseAmbientLight(const std::shared_ptr<DirectionLight>& light) { UseambientLight = light; }

        //分离(obj, 灯光, 点)渲染
        static void render(
            const std::vector<Object3DPtr>& objs,
            const std::vector<std::shared_ptr<SpotLight>>& spotights,
            const std::vector<std::shared_ptr<PointLight>>& pointlights
        );
        //渲染(3D)
        static void render(const std::vector<std::shared_ptr<Object3D>>& objs);
        
        static void RenderMeshEdge(const MeshPtr& mesh);

        static void RenderShapeEdge(const ShapePtr& shape, const glm::vec3& color);

        //初始化基本渲染着色器
        //  1.基本冯氏光照(带纹理)
        //  2.纯色着色(3D)
        //  3.纯纹理渲染(3D)
        //  4.纯色着色(2D)
        static void initDefaultShader();

        static void getAllNeedBlendColor();


        //渲染(2D)
        static void render(const std::vector<std::shared_ptr<Object2D>>& objs);
        //渲染碰撞盒体
        static void renderColliderBox(const ColliderPtr& box, const glm::vec3& color);
    
    private:
        static void RenderObj(const std::shared_ptr<Object3D>& obj,
        const std::vector<std::shared_ptr<SpotLight>> &spotlights, 
        const std::vector<std::shared_ptr<PointLight>> &pointlights);

        static void RenderObj(const std::shared_ptr<Object2D>& obj);

        static void caculateLight(    const std::vector<std::shared_ptr<SpotLight>>& spotights,
            const std::vector<std::shared_ptr<PointLight>>& pointlights
        );
        //查看渲染器是否渲染点/聚灯光的位置
        static bool IsLightRender() { return renderLight; }
        //设置灯光位置是否渲染
        static void SetLightRender(bool tag) { renderLight = tag;}

        static void getLights(
            const Object3DPtr& objs,
            std::vector<std::shared_ptr<SpotLight>>& spots,
            std::vector<std::shared_ptr<PointLight>>& points
        );
    private:
        //static RendererAPI mAPI;
        //临时存储一个静态的着色器指针，到时这里将改为一个map存储使用的着色器
        //static std::shared_ptr<Shader> UseShader;


        
        static std::unique_ptr<Shape>                       ball;

        static std::shared_ptr<DirectionLight>              UseambientLight;

        static bool                                         renderLight;

        static std::unique_ptr<Renderstatus>                 mstatuss;
    public:
        static std::map<string, std::shared_ptr<Shader>>    ShaderLib;

        static std::shared_ptr<Camera>                      UseCamera;

    };
}