#include "fspcs.h"
#include "RenderElement.h"
#include "FISH/Renderer/Buffer.h"

#include "Texture.h"
#include "FISH/Log.h"
#include "TextureManger.h"
#include "FISH/Timer.h"
#include "Animation.h"
#include "FISH/Debugger.h"

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
        int count = beginIndex;
        int idx = 0;
        //新建帧
        for (int i = 0; i < size; i++) {
            int frameIndex = beginIndex + i;
            string currentName = std::vformat(name, std::make_format_args(frameIndex));
            string texturePath = path + "/" + currentName;
            
            mFrames[i].duration = duration;

            FISH::TextureManager::get().loadTextureAsync(
                texturePath,
                Texture::TextureType::Texture2D,
                ChannelType::RGBA, 
                [this, i, size](TexturePtr ptr) {
                    FS_PROFILE_SCOPE("TextureLoadCallback");
                    if (ptr && ptr->isValid()) {
                        mFrames[i].setTexture(ptr);
                        mLoadedCount++;
                        FS_INFO("Load Count: {0}", mLoadedCount.load());
                        // 检查是否所有纹理都已加载完成
                        if (mLoadedCount.load() == size) {
                            FS_INFO("All the Frames were Loaded!");
                            mAllTexturesLoaded = true;
                            if (needPlayTag) {
                                FS_INFO("Ani Need To Play!!");
                                play(playMode);
                            }
                            FS_INFO("Animation '{}' all textures loaded successfully", mName);
                        }
                    } else {
                        FS_ERROR("Failed to load texture for animation frame {}", i);
                    }
            }
        );
    }

        mDuration = duration;
        
    }   

    SpriteAnimation::~SpriteAnimation() {
        //std::lock_guard<std::recursive_mutex> lock(mTimerMutex);
        if (TimerId) mAnimationTimer()->stopTimer(TimerId);
    }

    void SpriteAnimation::play(AnimationMode mode) {
        //std::lock_guard<std::recursive_mutex> lock(mTimerMutex);
        if (!mAllTexturesLoaded) {
            needPlayTag = 1;    
            return;
        }
        needPlayTag = 0;    
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
        FS_INFO("Begin Play Ani: {0}", mName);

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
        return mFrames[mCurrentFrame.load()].getTexture();
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
        return  mFrames[cf].getTexture()->getHandle();
    }

    unsigned long long SpriteAnimation::getTextureID() const {
        int cf = mCurrentFrame;
        if (cf >= mFrames.size()) cf = mFrames.size() - 1;
        return mFrames[cf].getTexture()->getTextureID();
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
        FS_PROFILE_SCOPE("FrameBeginCallback");
        if (!mAllTexturesLoaded) return;
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
        return mFrames[cf].getTexture()->getTextureInfo();
    }
}