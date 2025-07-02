#pragma once
#include "TextureHandle.h"
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

    class API_ SpriteAnimation : public TextureHandle {
    public: 
        SpriteAnimation();

        SpriteAnimation(string path, string name, int size, int beginIndex = 0, int duration = 1000, AnimationMode Mode = AnimationMode::Loop);

        ~SpriteAnimation();

        //是否完成
        bool IsFinsh();
        //是否暂停
        bool IsPause();
        //播放
        void play(AnimationMode mode);
        //停止
        void stop();
        //暂停
        void pause();
        //重置
        void reset();
        //设置从第几帧开始
        void setBeginInTexture(int idx);
        //获取当前帧
        std::shared_ptr<Texture> getCurrentFrame();
        //设置播放速度
        void setSpeed(float speed);
        //获取当前帧
        virtual unsigned long long getHandle() const override;
        //设置帧回调函数
        void setFrameCallBackFunc(const FrameCallFUN& func);
        //设置初始帧开始函数
        void setFrameBeginFunc(const FrameCallFUN& func);
        //设动画结束函数
        void setFrameEndFunc(const FrameCallFUN& func);
    private:
        //初始化计时器
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
        //设置帧启动函数
        FrameCallFUN                    mFrameBeginFunc;
        //设置帧结束函数        
        FrameCallFUN                    mFrameEndFunc;
    };

}



