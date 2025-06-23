#include "fspcs.h"
#include <fstream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "RenderElement.h"
#include "Texture.h"
#include "RenderState.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "FISH/Log.h"
#include "Font.h"

namespace FISH{
    static const int ATLAS_WIDTH = 1024;
    static const int ATLAS_HEIGHT = 1024;
    static const int FIRST_CHAR = 32;  // ASCII起始字符 (空格)
    static const int CHAR_COUNT = 96;  // 渲染字符数量 (32-126)

    Font::Font(const string &fontPath, float fontSize) {
        mFontSize = fontSize;
        LoadFont(fontPath, fontSize);
    }

    void Font::RenderText(const std::string &text, float x, float y, const glm::vec3 &color, float scale) {
        mAtlasTexture->bind();
        

    }

    std::shared_ptr<Texture> Font::getAtlasTexture() const {return mAtlasTexture;}

    const CharacterInfo &Font::getCharacterInfo(char32_t codepoint) {
        if (mCharacters.contains(codepoint)) {
            mCharacters[codepoint].lastUsedFrame = mCurrentFrame;
            return mCharacters[codepoint];
        } 

        AddCharacterToAtlas(codepoint);

        if (!mCharacters.contains(codepoint) || !mCharacters[codepoint].isLoaded) {
            static CharacterInfo spaceInfo;
            spaceInfo.advance = mFontSize * 0.5;
            return spaceInfo;
        }

        return mCharacters[codepoint];
    }

    void Font::LoadFont(const string &fontPath, float fontSize)
    {
        std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            FS_ERROR("Falied to open font file: {0}", fontPath);  
            return;
        }

        long long siz = file.tellg();
        file.seekg(0, std::ios::beg);
        
        mFontBuffer.resize(siz);
        if (!file.read((char*)mFontBuffer.data(), siz)) {
            FS_ERROR("Falied to read font file: {0}", fontPath);
            return;
        }

        if (!stbtt_InitFont(&mStbInfo, mFontBuffer.data(), 0)) {
            FS_ERROR("Falied to read initalize font: {0}", fontPath);
            return;
        }
        //计算缩放
        mScale = stbtt_ScaleForPixelHeight(&mStbInfo, fontSize);
        //重载数据
        mAtlasData.resize(ATLAS_WIDTH * ATLAS_HEIGHT, 0);

           // 初始化打包上下文
        if (!stbtt_PackBegin(&mPackContext, mAtlasData.data(), ATLAS_WIDTH, ATLAS_HEIGHT, 0, 1, nullptr)) {
            FS_CORE_ERROR("Failed to initialize font packing context");
            return;
        }

        mPackContextInitialized = true;

        stbtt_PackSetOversampling(&mPackContext, 1, 1);
        //默认32-128的字符
        for (int c = 32; c < 128; c++) {
            AddCharacterToAtlas(c);
        }
        //中文标点符号
        const char32_t commonPunctuation[] = {0x3000, 0x3001, 0x3002, 0xFF0C, 0xFF0E, 0xFF1F, 0xFF01, 0};
        for (int i = 0; commonPunctuation[i]; i++) {
            AddCharacterToAtlas(commonPunctuation[i]);
        }
    }

    void Font::CreateAtLasTexture() {
        mAtlasTexture.reset(Texture::CreateTextureFromMemory("FontAtlas", mAtlasData.data(), ATLAS_WIDTH, ATLAS_HEIGHT, ChannelType::Red));
    }

    void Font::AddCharacterToAtlas(char32_t codepoint) {
        //查看此字体是否加载
        if (mCharacters.contains(codepoint) && mCharacters[codepoint].isLoaded) {
            return;
        }
        //初始化字符信息
        auto& charInfo = mCharacters[codepoint];
        //获取字的度量
        int advance, lsb;
        float scale = stbtt_ScaleForMappingEmToPixels(&mStbInfo, mFontSize);
        stbtt_GetCodepointHMetrics(&mStbInfo, codepoint, &advance, &lsb);
        charInfo.advance = advance * scale;
        //字边界
        int x0, y0, x1, y1;
        stbtt_GetCodepointBitmapBox(&mStbInfo, codepoint, scale, scale, &x0, &y0, &x1, &y1);
        charInfo.size = glm::vec2(x1 - x0, y1 - y0);
        charInfo.bearing = glm::vec2(x0, -y0);

        //打包到位图中
        stbtt_packedchar packedChar;
        if (stbtt_PackFontRange(&mPackContext, mFontBuffer.data(), 0, mFontSize, codepoint, 1, &packedChar)){
            charInfo.texCoordsMin = glm::vec2(
                static_cast<float>(packedChar.x0) / ATLAS_WIDTH,
                static_cast<float>(packedChar.y0) / ATLAS_HEIGHT
            );

            charInfo.texCoordsMax = glm::vec2(
                static_cast<float>(packedChar.x1) / ATLAS_WIDTH,
                static_cast<float>(packedChar.y1) / ATLAS_HEIGHT
            );

            charInfo.isLoaded = true;
            charInfo.lastUsedFrame = mCurrentFrame;
            //更新纹理
            mAtlasTexture->updateSubTex(packedChar.x0, packedChar.y0, 
                packedChar.x1-packedChar.x0, packedChar.y1 - packedChar.y0, mAtlasData.data() + packedChar.y0 * ATLAS_WIDTH + packedChar.x0);
        }else {
            FS_CORE_WARN("Failed to pack character: U+{0:x}", (uint32_t)codepoint);
            charInfo.isLoaded = false;
        }

        
        
    }

    void Font::RenderQuad(float x, float y, float w, float h, float u1, float v1, float u2, float v2) {
        float vertices[] = {
            // 位置          // 纹理坐标
            x,     y + h,    u1, v2, // 左下 - 0
            x + w, y + h,    u2, v2, // 右下 - 1
            x + w, y,        u2, v1, // 右上 - 2
            x,     y,        u1, v1  // 左上 - 3
        };

        // 索引数据 (两个三角形组成四边形)
        unsigned int indices[] = {
            0, 1, 2, // 第一个三角形
            0, 2, 3  // 第二个三角形
        };

        std::shared_ptr<VertexBuffer> buffer(VertexBuffer::Create(vertices, sizeof(vertices)));
        buffer->SetLayout({
            {ShaderDataType::Float2, VertexType::Position},
            {ShaderDataType::Float2, VertexType::UV}
        });

        std::shared_ptr<IndexBuffer> indexbuffer(IndexBuffer::Create(indices, 6));
        std::shared_ptr<VertexArray> Vao(VertexArray::Create());

        Vao->AddVertexBuffer(buffer);
        Vao->SetIndexBuffer(indexbuffer); 
        
        Vao->renderIndex(0, TRIANGLES);
    }
}
