#include "stb_image.h"
#include "RenderElement.h"

int choiceStbChannel(ChannelType typ) {
    switch (typ) {
        case ChannelType::Red:  return STBI_grey;
        case ChannelType::RedA: return STBI_grey_alpha;
        case ChannelType::RGB:  return STBI_rgb;
        case ChannelType::RGBA: return STBI_rgb_alpha;
    }
    return STBI_default;
}

ChannelType StbChannelToEnumChannel(int typ) {
    switch(typ) {
        case STBI_grey: return ChannelType::Red;
        case STBI_grey_alpha: return ChannelType::RedA;
        case STBI_rgb: return ChannelType::RGB;
        case STBI_rgb_alpha: return ChannelType::RGBA;
    }
    return ChannelType::RGB;
}
