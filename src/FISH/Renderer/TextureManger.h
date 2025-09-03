#pragma once
#include "FISH/Core.h"
#include "RenderElement.h"
#include "Texture.h"

namespace FISH {
    //纹理管理器接口
    
    class API_ TextureManager {
        TextureManager();
    public:

        ~TextureManager();

        //异步加载
          void loadTextureAsync(
            const string& name, 
            ChannelType channel = ChannelType::RGBA, 
            const std::function<void(TexturePtr)>& callback = nullptr);

        //异步上载纹理
          void processAsyncUploads();

          TexturePtr getTexture(const string& name);

          bool contains(const string& name) const;

          void remove(const string& name);

          int count() const;

          void clean();

        static TextureManager& get();

    private:
            struct Task {
                string filePath;
                ChannelType channel;
                std::function<void(TexturePtr)> callback;
                std::vector<uint8_t> fileData;
                int width{0};
                int height{0};
                int channels{0};
            };
            //异步工线程
            void asyncLoadToMemory();
            //加载图片数据
            bool LoadImageData(Task& task);

    protected:
        mutable std::mutex mLoadQueMutex;
        mutable std::mutex mUploadQueMutex;
        std::queue<Task> mLoadQue;
        std::queue<Task> mUploadQue;
        std::condition_variable mAsyncCondition;
        std::thread mAsyncLoadThread;
        std::atomic<bool> mAsyncThreadRunning;
                
    };

}