#pragma once

#include "FISH/Core.h"

namespace FISH {
    enum class AudioType {
            AudioAll,  // 2D音频，不受3D空间影响
            Audio3D   // 3D音频，受空间位置影响
    };
    class API_ Sound {
    public:
        Sound(const string& path, AudioType Type, bool loop = false);

        ~Sound();
        //播放
        void play();
        //暂停
        void pause();
        //停止
        void stop();
        //重新设置
        void reset();
        //是否播放
        bool IsPlaying() const;

        //音量
        void setVolume(float volume);
        float getVolume() const;

        //音调
        void setPitch(float pitch);
        float getPitch() const;
        //设置音频位置
        void setPosition(const glm::vec3& pos);
        glm::vec3 getPosition() const;
        //设置音频移动方向（多普勒效应模拟）
        void setVelocity(const glm::vec3& dir);
        // 最小衰减距离
        void setMinDistance(float distance);  
        // 最大衰减距离
        void setMaxDistance(float distance);  
        // 衰减因子 
        void setRolloffFactor(float factor);   

        AudioType getAudioType() {return mType;}

        static void Initialize();
        static void Shutdown();
        static void SetListenerPosition(const glm::vec3& pos);
        static void SetListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
    private:
        void init();
        bool LoadAudioFile(const string& path);
    private:
        glm::vec3               mPosition{0.0};
        float                   mMinDistance{1.0};
        float                   mMaxDistance{1.0};
        float                   mFactor{1.0};
        float                   mPitch{1.0};
        float                   mVolume{1.0};
        bool                    playTag{0};
        AudioType               mType;
        unsigned int            mSource{0};
        unsigned int            mBuffer{0};
        bool                    initTag{0};
        
    };

}