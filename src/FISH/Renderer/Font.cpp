#include "fspcs.h"
#include <locale>
#include <codecvt>
#include "Renderstatus.h"
#include "RenderElement.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "FISH/Log.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include "Font.h"

namespace FISH {
    //计数类型
    int Font::Fontcounts = 0;
    //字体着色器
    std::unique_ptr<Shader>  Font::FontShader = nullptr;

    std::unique_ptr<Renderstatus>  Font::Fontstatus = nullptr;


    const int ATLAS_WIDTH = 1024*2;
    const int ATLAS_HEIGHT = 1024*2;
    //常用字符
    static const wchar_t* commonChars = L"的一是在不了有和人这中大为上个国我以要他时来用们生到作地于出就分对成会可主发年动同工也能下过子说产种面而方后多定行学法所民得经十三之进着等部度家电力里如水化高自二理起小物现实加量都两体制机当使点从业本去把性好应开它合还因由其些然前外天政四日那社义事平形相全表间样与关各重新线内数正心反你明看原又么利比或但质气第向道命此变条只没结解问意建月公无系军很情者最立代想已通并提直题党程展五果料象员革位入常文总次品式活设及管特件长求老头基资边流路级少图山统接知较将组见计别她手角期根论运农指几九区强放决西被干做必战先回则任取据处队南给色光门即保治北造百规热领七海口东导器压志世金增争济阶油思术极交受联什认六共权收证改清己美再采转更单风切打白教速花带安场身车例真务具万每目至达走积示议声报斗完类八离华名确才科张信马节话米整空元况今集温传土许步群广石记需段研界拉林律叫且究观越织装影算低持音众书布复容儿须际商非验连断深难近矿千周委素技备半办青省列习响约支般史感劳便团往酸历市克何除消构府称太准精值号率族维划选标写存候毛亲快效斯院查江型眼王按格养易置派层片始却专状育厂京识适属圆包火住调满县局照参红细引听该铁价严龙飞";
    //主要字体库
    static FT_Library ft = nullptr;
    
    static int currentX = 0;
    static int currentY = 0;
    static int maxRowHeight = 0;
    
    Font::Font(const string &fontpath) {
        Font::InitFTlibrary();
        Font::InitFontShader();
        Font::InitFontStatus();
        if (FT_New_Face(ft, fontpath.c_str(), 0, &mFace)) {
            FS_ERROR("Falied to load font!");
        }

        FT_Set_Pixel_Sizes(mFace, 0, mFontSize);
        FT_Select_Charmap(mFace, ft_encoding_unicode);

        mFontTexture = (Texture::CreateNullTexture(fontpath, ATLAS_WIDTH, ATLAS_WIDTH, ChannelType::Red, 1));

        for (int i = 0; i < wcslen(commonChars); ++i) {
            loadCharacter(commonChars[i]);
        }

        Font::Fontcounts++;
    }

    Font::~Font() {
        Font::Fontcounts--;
        if (mFace) {
            FT_Done_Face(mFace);
        }
        if (!Font::Fontcounts && ft) {
            FT_Done_FreeType(ft);
            ft = nullptr;
        }
    }

    void Font::RenderText(const string& text, float x, float y, float scale, const glm::vec3 &color) {
        Fontstatus->enablestatus(StatusType::Blend);
        Fontstatus->setstatusFunc(SetType::BlendFunc, FuncType::SrcAlpha, FuncType::MinusSrcAlpha);
        Fontstatus->disablestatus(StatusType::DepthTest);
        FontShader->Begin();
        FontShader->setTextureHandle("FontTex", mFontTexture->getHandle());
        FontShader->setVector3("color", color);
        GetTextBoxVao(text, x, y, scale)->renderIndex(0, TRIANGLES);
        FontShader->End(); 
        Fontstatus->enablestatus(StatusType::DepthTest);
        Fontstatus->disablestatus(StatusType::Blend);
    }

    std::shared_ptr<VertexArray> Font::GetTextBoxVao(const string &text, float x, float y, float scale){
        std::shared_ptr<VertexArray> Vao(VertexArray::Create());
        
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wideStr = converter.from_bytes(text);
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(text.size() * 4 * 4); 
        indices.reserve(text.size() * 6);

        float startX = x;
        unsigned int indexOffset = 0;

        for (auto c : wideStr) {
            if (!Characters.contains(c)) {
                loadCharacter(c);
                if (!Characters.contains(c)) continue;
            }

            const auto& ch = Characters[c];

            float xpos = startX + ch.Bearing.x * scale;
            float ypos = y - (ch.typeSize.y - ch.Bearing.y) * scale;
            
            float w = ch.typeSize.x * scale;
            float h = ch.typeSize.y * scale;
            
            // 当前字符的4个纹理坐标

            
            // 添加6个顶点构成2个三角形
            vertices.insert(vertices.end(), {
            // 位置          // 纹理坐标
            xpos,     ypos,     ch.texCoordsMin.x, ch.texCoordsMax.y, // 左下
            xpos + w, ypos,     ch.texCoordsMax.x, ch.texCoordsMax.y, // 右下
            xpos,     ypos + h, ch.texCoordsMin.x, ch.texCoordsMin.y, // 左上
            xpos + w, ypos + h, ch.texCoordsMax.x, ch.texCoordsMin.y  // 右上
            });
            
            indices.insert(indices.end(), {
                indexOffset, indexOffset + 1, indexOffset + 2,  // 第一个三角形
                indexOffset + 1, indexOffset + 3, indexOffset + 2   // 第二个三角形
            });

            indexOffset += 4;
            startX += (ch.Advance >> 6) * scale;
        }

        std::shared_ptr<VertexBuffer> Vbo(VertexBuffer::Create(vertices.data(), 4*vertices.size()));
        std::shared_ptr<IndexBuffer>  Index(IndexBuffer::Create(indices.data(), indices.size()));
        
        Vbo->SetLayout(
            {
                {ShaderDataType::Float2, VertexType::Position},
                {ShaderDataType::Float2, VertexType::UV}
            }
        );

        Vao->AddVertexBuffer(Vbo);
        Vao->SetIndexBuffer(Index);

        return Vao;
    }

    void Font::RenderTextFromVaoTextstd(const std::shared_ptr<VertexArray> &Vao, const std::shared_ptr<Texture> &tex, const glm::vec3 &color) {
        Fontstatus->enablestatus(StatusType::Blend);
        Fontstatus->setstatusFunc(SetType::BlendFunc, FuncType::SrcAlpha, FuncType::MinusSrcAlpha);
        Fontstatus->disablestatus(StatusType::DepthTest);
        FontShader->Begin();
        FontShader->setTextureHandle("FontTex", tex->getHandle());
        FontShader->setVector3("color", color);
        Vao->renderIndex(0, TRIANGLES);
        FontShader->End(); 
        Fontstatus->enablestatus(StatusType::DepthTest);
        Fontstatus->disablestatus(StatusType::Blend);
    }

    void Font::InitFTlibrary()
    {
        if (ft != nullptr) return;
        
        if (FT_Init_FreeType(&ft)) {
            FS_CORE_ERROR("Could not init FreeType Library");
        }
    }
    void Font::loadCharacter(wchar_t c) {
        if (Characters.contains(c)) return;

        if (FT_Load_Char(mFace, c, FT_LOAD_RENDER | FT_LOAD_TARGET_LIGHT)) {
            FS_CORE_ERROR("Failed to load Character: {0:x}", (int)c);
            return;
        }

        auto& glyph = mFace->glyph;
        auto& bitmap = glyph->bitmap;
        //更新插入位置
        if (currentX + bitmap.width > ATLAS_WIDTH) {
            currentX = 0;
            currentY += maxRowHeight;
            maxRowHeight = 0;

            if (currentY + bitmap.rows > ATLAS_HEIGHT) {
                FS_CORE_WARN("Texture atlas is full!");
                return;
            }
        }
        //更新图集
        mFontTexture->
            updateSubTex(currentX, currentY, bitmap.width, bitmap.rows, bitmap.buffer);
        

        //插入字体
        Characters[c] = 
            CharacterInfo(
            glm::vec2(currentX/(float)ATLAS_WIDTH, currentY/(float)ATLAS_HEIGHT),
            glm::vec2((currentX + bitmap.width)/(float)ATLAS_WIDTH, (currentY + bitmap.rows)/(float)ATLAS_HEIGHT),
            glm::ivec2(bitmap.width, bitmap.rows),
            glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
            static_cast<int>(glyph->advance.x));

        // 更新位置
        currentX += bitmap.width;
        maxRowHeight = max(maxRowHeight, bitmap.rows);
    }
    void Font::InitFontShader() {
        if (FontShader != nullptr) return;
        
        FontShader.reset(Shader::CreateShader());

        FontShader->readVertexShader("sharders/EnginRenderShader/FontVertex.glsl");
        FontShader->readFragmentShader("sharders/EnginRenderShader/FontFragment.glsl");
        FontShader->CompileLink();
    }
    void Font::InitFontStatus() {
        if (Fontstatus != nullptr) return;

        Fontstatus.reset(Renderstatus::CreateRenderstatus());
    }
}