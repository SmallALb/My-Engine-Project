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
