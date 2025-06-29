#pragma once

#include "FISH/Core.h"

namespace FISH {
    using FrameCallFUN = std::function<void()>;
    enum class AnimationMode {
        Loop,
        Once,
        PingPong
    };

    struct AnimationFrame {
        //纹理
        std::shared_ptr<Texture> texture;
        //持续时间
        int duration;

        AnimationFrame() {};

        AnimationFrame(const std::shared_ptr<Texture>& tx, int dura):
            texture(tx), duration(dura)
        {}
    };

    class API_ SpriteAnimation : public std::enable_shared_from_this<SpriteAnimation> {
    public: 
        SpriteAnimation();

        SpriteAnimation(string path, string name, int size, int beginIndex = 0, int duration = 1000, AnimationMode Mode = AnimationMode::Loop);

        ~SpriteAnimation();

        //是否完成
        bool IsFinsh();
        //播放
        void play(AnimationMode mode);
        //停止
        void stop();
        //暂停
        void pause();
        //重置
        void reset();
        //获取当前帧
        std::shared_ptr<Texture> getCurrentFrame();
        //设置播放速度
        void setSpeed(float speed);

        unsigned long long getCurrentHandle();
    private:
        void initTimer();
    private:
        //当前帧
        std::atomic_int                 mCurrentFrame{0};
        //该动画的所有帧实现
        std::vector<AnimationFrame>     mFrames;
        //帧数量
        std::atomic_int                 FrameSize;
        //帧间隔时间（ms）
        int                             mDuration{100};
        //播放模式
        AnimationMode                   playMode;
        //播放结束标志
        std::atomic_bool                FishTag{0};
        //停止标志
        std::atomic_bool                PauseTag{0};
        //播放速度
        float                           mSpeed{1.0};
        //计时器id
        int                             TimerId{0};
        //帧回调函数
        FrameCallFUN                    mFrameCallFunc;
        //帧自增函数
        FrameCallFUN                    mFrameIncreaseFunc;
    };

}



