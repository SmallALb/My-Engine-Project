#include "fspcs.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include "FISH/Log.h"
#include "Sound.h"


namespace FISH {
    ALCdevice* g_Device = nullptr;
    ALCcontext* g_Context = nullptr;

    Sound::Sound(const string& path, AudioType Type, bool loop) {
        mType = Type;

        alGenSources(1, &mSource);
        alGenBuffers(1, &mBuffer);

        if (LoadAudioFile(path) == false) {
            FS_CORE_ERROR("Load Audio Failed!");
            return;
        }

        init();

        alSourcei(mSource, AL_LOOPING, loop);
        initTag = 1;
    }

    Sound::~Sound() {
        if (initTag) {
            stop();
            alDeleteSources(1, &mSource);
            alDeleteBuffers(1, &mBuffer);
        }
    }

    void Sound::play() {
        if (initTag) {
            alSourcePlay(mSource);
            playTag = 1;
        }
    }


    void Sound::pause() {
        if (initTag) {
            alSourcePause(mSource);
            playTag = 0;
        }
    }

    void Sound::stop() {
        if (initTag) {
            alSourceStop(mSource);
            playTag = 0;
        }
    }


    void Sound::reset() {
        if (initTag) {
            alSourceRewind(mSource);
        }
    }

    bool Sound::IsPlaying() const {
        return playTag;
    }

    void Sound::setVolume(float volume) {
        if (initTag) {
            mVolume = volume;
            alSourcef(mSource, AL_GAIN, mVolume);
        }
    }
    
    void Sound::init() {
        alSourcef(mSource, AL_PITCH, mPitch);
        alSourcef(mSource, AL_GAIN, mVolume);
          // 通用属性
        alSourcef(mSource, AL_PITCH, 1.0f);
        alSourcef(mSource, AL_GAIN, 1.0f);
        
        // 根据音频类型设置不同属性
        if (mType == AudioType::AudioAll) {
            // 2D音频设置
            alSourcei(mSource, AL_SOURCE_RELATIVE, 1);  // 相对于监听者
            alSource3f(mSource, AL_POSITION, 0, 0, 0);        // 与监听者同位置
            alSourcef(mSource, AL_ROLLOFF_FACTOR, 0.0f);      // 禁用衰减
            FS_INFO("ALL Audio");
        } else {
            // 3D音频设置
            alSourcei(mSource, AL_SOURCE_RELATIVE, 0); // 绝对坐标
            alSource3f(mSource, AL_POSITION, 0, 0, 0);        // 初始位置 
            alSourcef(mSource, AL_REFERENCE_DISTANCE, mMinDistance);  // 参考距离
            alSourcef(mSource, AL_ROLLOFF_FACTOR, mFactor);      // 默认衰减因子
            alSourcef(mSource, AL_MAX_DISTANCE, mMaxDistance);      // 最大距离
            FS_INFO("3D Audio");
        }
    }

    bool Sound::LoadAudioFile(const string &path) {
        SF_INFO sfInfo;
        if (!std::filesystem::exists(path)) {
            FS_CORE_ERROR("File does not exist: {0}", path.c_str());
            return false;
        }

        SNDFILE* file = sf_open(path.c_str(), SFM_READ, &sfInfo);
        if (!file) {
            FS_CORE_ERROR("Failed to open file: {0}", path.c_str());
            return false;
        }

         // 确定OpenAL格式
        ALenum format;
         switch(sfInfo.channels) {
            case 1:  // 单声道
                format = (sfInfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_U8 ? 
                        AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
                break;
            case 2:  // 立体声
                format = (sfInfo.format & SF_FORMAT_SUBMASK) == SF_FORMAT_PCM_U8 ? 
                        AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
                break;
            case 4:  // 四声道(Quad)
                format = alGetEnumValue("AL_FORMAT_QUAD16");
                if (format == AL_NONE) {
                    FS_CORE_ERROR("Quad channel format not supported");
                    sf_close(file);
                    return false;
                }
                break;
            case 6:  // 5.1环绕声
                format = alGetEnumValue("AL_FORMAT_51CHN16");
                if (format == AL_NONE) {
                    FS_CORE_ERROR("5.1 channel format not supported");
                    sf_close(file);
                    return false;
                }
                break;
            case 7:  // 6.1环绕声
                format = alGetEnumValue("AL_FORMAT_61CHN16");
                if (format == AL_NONE) {
                    FS_CORE_ERROR("6.1 channel format not supported");
                    sf_close(file);
                    return false;
                }
                break;
            case 8:  // 7.1环绕声
                format = alGetEnumValue("AL_FORMAT_71CHN16");
                if (format == AL_NONE) {
                    FS_CORE_ERROR("7.1 channel format not supported");
                    sf_close(file);
                    return false;
                }
                break;
            default:
                FS_CORE_ERROR("Unsupported channel count: {0}", sfInfo.channels) ;
                sf_close(file);
                return false;
        }

        // 读取音频数据
        std::vector<short> data(sfInfo.frames * sfInfo.channels);
        sf_readf_short(file, data.data(), sfInfo.frames);
        sf_close(file);

        // 加载到OpenAL缓冲区
        alBufferData(mBuffer, format, data.data(), 
                    data.size() * sizeof(short), sfInfo.samplerate);
        if (alGetError() != AL_NO_ERROR) {
            FS_CORE_ERROR("Failed to load audio data into buffer") ;
            return false;
        }

        // 将缓冲区附加到源
        alSourcei(mSource, AL_BUFFER, mBuffer);
        return true;
    }

    float Sound::getVolume() const {
        return mVolume;
    }
    
    void Sound::setPitch(float pitch) {
        if (initTag) {
            mPitch = pitch;
            alSourcef(mSource, AL_PITCH, mPitch);
        }
    }

    float Sound::getPitch() const {
        return mPitch;
    }

    void Sound::setVelocity(const glm::vec3 &dir) {
        if (initTag && mType == AudioType::Audio3D) {
            alSource3f(mSource, AL_VELOCITY, dir.x, dir.y, dir.z);
        }
    }

    void Sound::setMinDistance(float distance) {
        if (initTag && mType == AudioType::Audio3D) {
            mMinDistance = distance;
            alSourcef(mSource, AL_REFERENCE_DISTANCE, distance);
        }
    }

    void Sound::setMaxDistance(float distance) {
        if (initTag && mType == AudioType::Audio3D) {
            mMaxDistance = distance;
            alSourcef(mSource, AL_MAX_DISTANCE, distance);
        }
    }

    void Sound::setRolloffFactor(float factor) {
        if (initTag && mType == AudioType::Audio3D) {
            mFactor = factor;
            alSourcef(mSource, AL_ROLLOFF_FACTOR, factor);
        }
    }

    void Sound::setPosition(const glm::vec3& pos) {
        if (initTag && mType == AudioType::Audio3D) {
            mPosition = pos;
            alSource3f(mSource, AL_POSITION, pos.x, pos.y, pos.z);
        }
    }

    glm::vec3 Sound::getPosition() const {
        return mPosition;
    }

    void Sound::Initialize() {
          g_Device = alcOpenDevice(nullptr);
        if (!g_Device) {
           FS_CORE_ERROR( "Failed to open OpenAL device");
            return;
        }

        g_Context = alcCreateContext(g_Device, nullptr);
        if (!g_Context) {
            FS_CORE_ERROR("Failed to create OpenAL context");
            alcCloseDevice(g_Device);
            return;
        }

        alcMakeContextCurrent(g_Context);
        alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
        // 设置默认监听者属性
        SetListenerPosition({0, 0, 0});
        SetListenerOrientation({0, 0, -1}, {0, 1, 0});
    }

    void Sound::Shutdown() {
        alcMakeContextCurrent(nullptr);
        if (g_Context) {
            alcDestroyContext(g_Context);
        }
        if (g_Device) {
            alcCloseDevice(g_Device);
        }
    }
    void Sound::SetListenerPosition(const glm::vec3& pos) {
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }

    void Sound::SetListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
        float ori[] = {
            forward.x, forward.y, forward.z,
            up.x,       up.y,        up.z
        };
        alListenerfv(AL_ORIENTATION, ori);
    }
}