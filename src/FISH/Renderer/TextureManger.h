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
        //(立方体贴图请遵循右-左-上-底-后-前的顺序来传入)
        void loadTextureAsync(
            const std::variant<string, std::array<string, 6>>& name, 
            Texture::TextureType typ = Texture::TextureType::Texture2D,
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
                std::variant<string, std::array<string, 6>> filePath;
                Texture::TextureType CreateType;
                ChannelType channel;
                std::function<void(TexturePtr)> callback;
                std::variant<std::vector<uint8_t>, std::array<std::vector<uint8_t>, 6>> fileData;
                int width{0};
                int height{0};
                int channels{0};
            };
            //异步工线程
            void asyncLoadToMemory();
            //加载图片数据
            bool LoadImageData(Task& task);
            //加载2d纹理
            bool load2D(Task& task);
            //加载立方体纹理
            bool loadCube(Task& task);

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