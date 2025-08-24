#include "fspcs.h"
#include "RenderElement.h"
#include "Texture.h"
#include "FISH/Log.h"
#include "FISH/Timer.h"
#include "Animation.h"


namespace FISH {
    Timer * mAnimationTimer() {
        static Timer timer;
        return &timer;
    }

    SpriteAnimation::SpriteAnimation() {

    }

    SpriteAnimation::SpriteAnimation(string path, string name, int size, int beginIndex, int duration, AnimationMode Mode) {
        

        mFrameCallFunc = [&]() {};
        mFrameBeginFunc = [&]() {};
        mFrameEndFunc = [&]() {};

        playMode = Mode;
        mName = name;
        mBeginIndex = beginIndex;
        mFrames.resize(size);
        FrameSize = size;
        mPath = path;
        path += "/" + name;
        int count = beginIndex;
        //新建帧
        for (auto& frame : mFrames) {
            AnimationFrame nFrame(Texture::CreateTextureFromPath(path + "(" + std::to_string(count) + ").png"),duration);
            frame = std::move(nFrame);
            count++;
        }

        mDuration = duration;
        
    }   

    SpriteAnimation::~SpriteAnimation() {
        //std::lock_guard<std::recursive_mutex> lock(mTimerMutex);
        if (TimerId) mAnimationTimer()->stopTimer(TimerId);
    }

    void SpriteAnimation::play(AnimationMode mode) {
        //std::lock_guard<std::recursive_mutex> lock(mTimerMutex);
        if (TimerId) stop();
        reset();
        PauseTag = false;
        FishTag = false;
        playMode = mode;

        TimerId =  mAnimationTimer()->createTimer(
            int(mDuration/mSpeed), 
            [this]() {
                if (PauseTag || FishTag) return;
                increseToNextFrame();
            }, 
            TimerMode::REPEATING
        );

    }

    void SpriteAnimation::reset() {
        mCurrentFrame = 0;
    }

    void SpriteAnimation::setBeginInTexture(int idx) {
        mCurrentFrame = idx;
    }

    void SpriteAnimation::stop() {
        std::lock_guard<std::recursive_mutex> lock(mTimerMutex);
        mAnimationTimer()->stopTimer(TimerId);
        //reset();
        TimerId = 0;
        FishTag = 1;
    }

    bool SpriteAnimation::IsFinsh() {
        return FishTag.load();
    }

    bool SpriteAnimation::IsPause() {
        return PauseTag;
    }

    void SpriteAnimation::pause(){
        PauseTag = 1;
    }

    std::shared_ptr<Texture> SpriteAnimation::getCurrentFrame() {
        return mFrames[mCurrentFrame.load()].texture;
    }

    void SpriteAnimation::setSpeed(float speed) {
        //std::lock_guard<std::recursive_mutex> lock(mTimerMutex);
        if (speed <= 0) return;
        mSpeed = speed;
        if (TimerId) 
            mAnimationTimer()->modifyTimer(
                TimerId,
                ((int)(mDuration / mSpeed)),
                [this]() {increseToNextFrame(); },
                TimerMode::REPEATING
            );
        
    }

    unsigned long long SpriteAnimation::getHandle() const {
        int cf = mCurrentFrame.load();
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

    void SpriteAnimation::increseToNextFrame() {

        bool isFirstFrame = (mCurrentFrame == 0);
        if (isFirstFrame && mFrameBeginFunc) mFrameBeginFunc();

        if (mFrameCallFunc) mFrameCallFunc();
        mCurrentFrame++;
        if (mCurrentFrame >= FrameSize) {
            if (mFrameEndFunc) mFrameEndFunc();
            FishTag = (playMode == AnimationMode::Once && mCurrentFrame >= FrameSize);
            if (FishTag) stop();
            else if(mCurrentFrame >= FrameSize) reset();
        }
    }

    void SpriteAnimation::reduceToLastFrame()
    {
    }
    
    TextureInfo SpriteAnimation::getTextureInfo() const {
        int cf = mCurrentFrame;
        if (cf >= mFrames.size()) cf = mFrames.size() - 1;
        return mFrames[cf].texture->getTextureInfo();
    }
}