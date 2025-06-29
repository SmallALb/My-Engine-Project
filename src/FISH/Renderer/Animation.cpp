#include "fspcs.h"
#include "RenderElement.h"
#include "Texture.h"
#include "FISH/Log.h"
#include "FISH/Timer.h"
#include "Animation.h"

namespace FISH {
    //帧计时器
    static std::unique_ptr<Timer>  mAnimationTimer{nullptr};

    SpriteAnimation::SpriteAnimation() {

    }

    SpriteAnimation::SpriteAnimation(string path, string name, int size, int beginIndex, int duration, AnimationMode Mode) {
        mFrameIncreaseFunc = [&]() {
            if (PauseTag || FishTag) return;
            mCurrentFrame++;
            if (mCurrentFrame >= FrameSize) {
                mCurrentFrame = 0;
                FishTag = playMode == AnimationMode::Once;
            }
        };

        mFrameCallFunc = [&]() {};

        playMode = Mode;

        mFrames.resize(size);
        FrameSize = size;
        path += "/" + name;
        int count = beginIndex;
        //新建帧
        for (auto& frame : mFrames) {
            AnimationFrame nFrame(std::shared_ptr<Texture>(Texture::CreateTextureFromPath(path + "(" + std::to_string(count) + ").png")),duration);
            frame = std::move(nFrame);
            count++;
        }

        mDuration = duration;
        
    }   

    SpriteAnimation::~SpriteAnimation() {

    }

    void SpriteAnimation::play(AnimationMode mode) {
        PauseTag = 0;
        //防止多次启动计时器
        if (!TimerId || FishTag){
            initTimer();
            auto self = shared_from_this(); 
            //启动计时器
            TimerId =  mAnimationTimer->createTimer(int(mDuration*mSpeed), 
                [self]() {
                    self->mFrameIncreaseFunc();
                    self->mFrameCallFunc();
                }, 
            mode == AnimationMode::Loop ? TimerMode::REPEATING : TimerMode::SINGLE);
        }
        
    }

    void SpriteAnimation::reset() {
        mCurrentFrame = 0;
    }

    void SpriteAnimation::stop() {
        mAnimationTimer->stopTimer(TimerId);
        TimerId = 0;
    }

    bool SpriteAnimation::IsFinsh() {
        return FishTag;
    }

    void SpriteAnimation::pause(){
        PauseTag = 1;
    }

    std::shared_ptr<Texture> SpriteAnimation::getCurrentFrame() {
        return mFrames[mCurrentFrame].texture;
    }

    void SpriteAnimation::setSpeed(float speed) {
        mSpeed = speed;
    }
    unsigned long long SpriteAnimation::getCurrentHandle() {
        int cf = mCurrentFrame;
        //FS_INFO("{0}", cf);
        if (cf >= mFrames.size()) cf = mFrames.size() - 1;
        return mFrames[cf].texture->getHandle();
    }
    void SpriteAnimation::initTimer() {
        if (mAnimationTimer != nullptr) return;
        mAnimationTimer.reset(new Timer()); 
    }
}