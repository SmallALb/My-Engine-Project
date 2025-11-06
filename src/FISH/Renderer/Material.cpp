#include "fspcs.h"
#include "Material.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "Texture.h"
namespace FISH {
    Material::Material() {
        Texture::initDefaultTexture();
        textures[TextureType::None] = Texture::NoneTexture;
    }
    Material::~Material() {}
    void Material::setTexture(TextureType typ, const TextureHandlePtr &ptr) {
        textures[typ] = ptr;
    }
    unsigned long long Material::getTextureHandle(TextureType typ) const {
        if (!textures.contains(typ) || textures.at(typ) == nullptr) {
            FS_INFO("Not this type of Texture");
            return 0;
        }
        return textures.at(typ)->getHandle();
    }
}