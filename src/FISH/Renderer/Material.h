#pragma once

#include "Renderstatus.h"
#include "TextureHandle.h"
#include "FISH/Core.h"

namespace FISH {
    enum class TextureType {
        None,           //普通贴图
        Mask,           //蒙版
        Normal,         //法线
        Alpha,          //透明通道贴图
        Specular,       //高光
        Height,         //高度
        Cubemap,        //立方体
        Metallic       //金属度
    };

    //材质类
    class API_ Material {
    public:
        Material();

        ~Material();

        void setTexture(TextureType typ, const TextureHandlePtr& ptr);

        unsigned long long getTextureHandle(TextureType typ) const;

        inline void setDepthWriteTag(bool tag) {enableDepthWrite = tag;}

        inline void setBlendColorTag(bool tag) {enableBlendColor = tag;}

        inline bool isDepthWriteEnable() const {return enableDepthWrite;}

        inline bool isBlendColorEnable() const {return enableBlendColor;}

        inline FuncType getSrcAlpthaBlendMode() const {return SrcAlpthaBlendMode;}

        inline FuncType getDstAlpthBlendMode() const {return DstAlpthBlendMode;}

        inline FuncType getDepthTestMode() const {return DepthTestMode;}

        inline void setSrcAlpthaBlendMode(FuncType mode) {SrcAlpthaBlendMode = mode;}

        inline void setDstAlpthBlendMode(FuncType mode) {DstAlpthBlendMode = mode;}

        inline void setDepthTestMode(FuncType mode) {DepthTestMode = mode;}


    private:
        std::unordered_map<TextureType, TextureHandlePtr> textures;

        //深度写入
        bool                          enableDepthWrite{0};
        //颜色混合
        bool                          enableBlendColor{0};
        //深度写入的模式
        FuncType                      DepthTestMode{FuncType::Less};
        //透明通道混合模式（作为源）
        FuncType                      SrcAlpthaBlendMode{FuncType::SrcAlpha};
        //透明通道混合模式（对于目标）
        FuncType                      DstAlpthBlendMode{FuncType::MinusSrcAlpha};
    };


    using MaterialPtr  = std::shared_ptr<Material>;
}