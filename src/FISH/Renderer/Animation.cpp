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
            if (mCurrentFrame == 0) mFrameBeginFunc();
            mCurrentFrame++;
            if (mCurrentFrame >= FrameSize) {
                mCurrentFrame = 0;
                mFrameEndFunc();
                FishTag = playMode == AnimationMode::Once;
            }
        };
        

        mFrameCallFunc = [&]() {};
        mFrameBeginFunc = [&]() {};
        mFrameEndFunc = [&]() {};

        playMode = Mode;

        mFrames.resize(size);
        FrameSize = size;
        path += "/" + name;
        mPath = path;
        int count = beginIndex;
        //新建帧
        for (auto& frame : mFrames) {
            FS_INFO("frame({0})", count);
            AnimationFrame nFrame(Texture::CreateTextureFromPath(path + "(" + std::to_string(count) + ").png"),duration);
            frame = std::move(nFrame);
            count++;
        }

        mDuration = duration;
        
    }   

    SpriteAnimation::~SpriteAnimation() {
        if (TimerId) mAnimationTimer->stopTimer(TimerId);
    }

    void SpriteAnimation::play(AnimationMode mode) {
        PauseTag = 0;
        //防止多次启动计时器
        if (!TimerId || FishTag){
            initTimer();
            //启动计时器
            TimerId =  mAnimationTimer->createTimer(int(mDuration*mSpeed), 
                [this]() {
                    this->mFrameIncreaseFunc();
                    this->mFrameCallFunc();
                }, 
            mode == AnimationMode::Loop ? TimerMode::REPEATING : TimerMode::SINGLE);
            FishTag = 0;
        }
        playMode = mode;
    }

    void SpriteAnimation::reset() {
        mCurrentFrame = 0;
    }

    void SpriteAnimation::setBeginInTexture(int idx) {
        mCurrentFrame = idx;
    }

    void SpriteAnimation::stop() {
        mAnimationTimer->stopTimer(TimerId);
        TimerId = 0;
    }

    bool SpriteAnimation::IsFinsh()
    {
        return FishTag;
    }

    bool SpriteAnimation::IsPause() {
        return PauseTag;
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

    unsigned long long SpriteAnimation::getHandle() const {
        int cf = mCurrentFrame;
        if (cf >= mFrames.size()) cf = mFrames.size() - 1;
        return mFrames[cf].texture->getHandle();
    }

    unsigned long long SpriteAnimation::getTextureID() const {
        int cf = mCurrentFrame;
        if (cf >= mFrames.size()) cf = mFrames.size() - 1;
        return mFrames[cf].texture->getTextureID();
    }

    void SpriteAnimation::setFrameCallBackFunc(const FrameCallFUN &func) {
        mFrameCallFunc = func;
    }

    void SpriteAnimation::setFrameBeginFunc(const FrameCallFUN &func) {
        mFrameBeginFunc = func;
    }

    void SpriteAnimation::setFrameEndFunc(const FrameCallFUN &func) {
        mFrameEndFunc = func;
    }

    void SpriteAnimation::initTimer() {
        if (mAnimationTimer != nullptr) return;
        mAnimationTimer.reset(new Timer());
    }
}