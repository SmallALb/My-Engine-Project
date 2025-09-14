#pragma once

#include "FISH/Core.h"

namespace FISH{
    struct CharacterInfo {
        // 纹理左下角坐标
        glm::vec2 texCoordsMin;
        // 纹理右上角坐标  
        glm::vec2 texCoordsMax;  
        //字体大小
        glm::ivec2 typeSize;
        //起点位置
        glm::ivec2 Bearing;
        //距离下一个字的起点距离
        int Advance;

        CharacterInfo() {}
        //带参构造
        CharacterInfo(
                const glm::vec2& Min,
                const glm::vec2& Max,
                const glm::ivec2& _size, 
                const glm::ivec2& _bearing, 
                int advance): 
            texCoordsMin(Min), texCoordsMax(Max), typeSize(_size), Bearing(_bearing), Advance(advance)
        {}
    };
    
    class API_ Font {
    public:
        Font(const string& fontpath);

        ~Font();

        void RenderText(const string& text, float x, float y, float scale, const glm::vec3& color);
    
        std::shared_ptr<VertexArray> GetTextBoxVao(const string& text, float x, float y, float scale);

        static void RenderTextFromVaoTextstd(const std::shared_ptr<VertexArray>& Vao, const std::shared_ptr<Texture>& tex, const glm::vec3 &color);
    private:
        //初始化字体库
        static void InitFTlibrary();
        //加载字体
        void loadCharacter(wchar_t c);
        //初始化字体渲染着色器
        static void InitFontShader();
        //初始化字体状态器
        static void InitFontStatus();
    private:
        //字体编码内存映射
        std::map<wchar_t, CharacterInfo> Characters;
        //字体加载器
        FT_Face mFace;
        //字体纹理
        std::shared_ptr<Texture> mFontTexture;
        //字体大小
        int mFontSize{64};

        //计数类型
        static int Fontcounts;
        //字体着色器
        static std::unique_ptr<Shader>  FontShader;

        static std::unique_ptr<Renderstatus>  Fontstatus;
    };


}