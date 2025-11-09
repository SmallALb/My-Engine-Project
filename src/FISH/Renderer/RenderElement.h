#pragma once
//shape
#define POINTS 0x0000
#define LINES 0x0001
#define LINE_LOOP 0x0002
#define LINE_STRIP 0x0003
#define TRIANGLES 0x0004
#define TRIANGLE_STRIP 0x0005
#define TRIANGLE_FAN 0x0006

enum class ElementType {
    points,
    lines,
    line_loop,
    line_strip,
    triangles,
    triangles_strip,
    triangles_fan
}; 

//通道
enum class ChannelType {
    Red,
    RedA,
    RGB,
    RGBA
};

int choiceStbChannel(ChannelType typ);

ChannelType StbChannelToEnumChannel(int typ);
