#pragma once

#include "FISH/Core.h"

namespace FISH {
    //字符信息
    struct CharacterInfo {
        glm::vec2 size;
        glm::vec2 bearing;
        glm::vec2 texCoordsMin;
        glm::vec2 texCoordsMax;
        float advance;
        bool isLoaded = false;   // 是否已加载到纹理图集
        int lastUsedFrame = 0;   // 最后使用帧数 (用于LRU缓存)

        CharacterInfo() {}

        CharacterInfo(
            const glm::vec2& _size,
            const glm::vec2& _bearing,
            const glm::vec2& _texCoordsMin,
            const glm::vec2& _texCoordsMax,
            float _advance
        ): 
            size(_size),
            bearing(_bearing),
            texCoordsMin(_texCoordsMin),
            texCoordsMax(_texCoordsMax)
        {};
    };
    
    //字体类
    class API_ Font {
    public:
        Font() {}

        Font(const string& fontPath, float fontSize);

        ~Font() {}

        void RenderText(const std::string& text, float x, float y, 
                   const glm::vec3& color = glm::vec3(1.0f), 
                   float scale = 1.0f);
        //取得字体纹理
        std::shared_ptr<Texture> getAtlasTexture() const;
        //取得字体信息
        const CharacterInfo& getCharacterInfo(char32_t codepoint);
    private:
        //加载字体
        void LoadFont(const string& fontPath, float fontSize );
        //创建字体纹理
        void CreateAtLasTexture();
        //添加新字符
        void AddCharacterToAtlas(char32_t codepoint);
        //设置字体宽高位置uv
        void RenderQuad(float x, float y, float w, float h, 
                   float u1, float v1, float u2, float v2);
    private:
        float                                           mFontSize{0.0f};
        float                                           mScale{1.0f};
        int                                             mCurrentFrame{0};
        std::unique_ptr<RenderState>                    mState{nullptr};
        stbtt_fontinfo                                  mStbInfo;
        std::vector<unsigned char>                      mFontBuffer;
        std::vector<unsigned char>                      mAtlasData;
        std::unordered_map<char, CharacterInfo>         mCharacters{};
        std::shared_ptr<Texture>                        mAtlasTexture{nullptr};
        stbtt_pack_context                              mPackContext;
        bool                                            mPackContextInitialized{false};
    };


}