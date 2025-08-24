#pragma once
#include "TextureHandle.h"
#include "FISH/Core.h"

namespace FISH {
    Timer* mAnimationTimer();

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

        AnimationMode getCurrentPlayMode() const {return playMode;}
        //获取当前帧
        std::shared_ptr<Texture> getCurrentFrame();
        //获取当前帧编号
        inline int getCurrentFrameIndex() const {
            return mCurrentFrame.load();
        }
        //获取帧总数
        inline int getFramesCount() const {
           return mFrames.size();
        }
        //设置播放速度
        void setSpeed(float speed);
        //获取当前帧
        virtual unsigned long long getHandle() const override;
        //获取ID
        virtual unsigned long long getTextureID() const override;
        //设置帧回调函数
        void setFrameCallBackFunc(const FrameCallFUN& func);
        //设置初始帧开始函数
        void setFrameBeginFunc(const FrameCallFUN& func);
        //设动画结束函数
        void setFrameEndFunc(const FrameCallFUN& func);
        //帧自增
        void increseToNextFrame();
        //帧自减
        void reduceToLastFrame();   

        TextureInfo getTextureInfo() const;

        virtual TextureHandleType getType() const {return TextureHandleType::Dynamic;}

        string getName() const {return mName;}

        int getAnimationIndex() const {return mBeginIndex;}

        int getDuration() const {return mDuration;}

        int Size() const {return FrameSize;}
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
        //设置帧启动函数
        FrameCallFUN                    mFrameBeginFunc;
        //设置帧结束函数        
        FrameCallFUN                    mFrameEndFunc;
        //动画的名称
        string                          mName;
        //
        int                             mBeginIndex{-1};
        //锁
        mutable std::recursive_mutex      mTimerMutex;
    };

}



