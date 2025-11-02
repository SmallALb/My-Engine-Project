#include "fspcs.h"
#include "FISH/Log.h"
#include "TextureManger.h"
#include "Texture.h"
#include "FISH/Renderer/stb_image.h"

namespace FISH {

    TextureManager::TextureManager() {
        mAsyncLoadThread = std::thread(&TextureManager::asyncLoadToMemory, this);
        mAsyncThreadRunning = true;
        FS_INFO("TecxtureManger Build!");
    }

    TextureManager::~TextureManager() {
        mAsyncThreadRunning = false;
        mAsyncCondition.notify_all();
        if (mAsyncLoadThread.joinable()) {
            mAsyncLoadThread.join();
        }
    }

    TextureManager& TextureManager::get() {
        static TextureManager manager;
        return manager;
    }

    void TextureManager::asyncLoadToMemory() {
        while(mAsyncThreadRunning) {
            Task task;

            {
                FS_INFO("Waiting for loads");
                std::unique_lock<std::mutex> lock(mLoadQueMutex);
                mAsyncCondition.wait(lock, [this]() {
                    return !mLoadQue.empty() || !mAsyncThreadRunning;
                });
                FS_INFO("Done! Having loads push in Que!");
                if (!mAsyncThreadRunning) break;
                if (mLoadQue.empty()) continue;

                task = mLoadQue.front();
                mLoadQue.pop();
            }

            if (LoadImageData(task)) {
                std::lock_guard<std::mutex> lock(mUploadQueMutex);
                mUploadQue.push(std::move(task));
                FS_INFO("Successed To LoadImageData In Task!");
            }
            else {
                FS_INFO("Failed To LoadImageData In Task!");
            }
        }
    }

    bool TextureManager::LoadImageData(Task &task) {
        switch(task.CreateType) {
            case Texture::TextureType::Texture2D:
                return load2D(task);
            case Texture::TextureType::TextureCube:
                return loadCube(task);
        }
        return false;
    }

    bool TextureManager::load2D(Task &task) {
        stbi_set_flip_vertically_on_load(true);

        int w, h, c;
        auto filename = std::get<0>(task.filePath);
        unsigned char* data = stbi_load(filename.c_str(), &w, &h, &c, 0);

        if (!data) {
            FS_CORE_ERROR("Failed to load image: {0}", filename);
            return false;
        }

        task.width = w;
        task.height = h;
        task.channels = c;

        size_t dataSize = w * h * c;
        auto* filedata = std::get_if<0>(&task.fileData);
        filedata->assign(data, data + dataSize);
        stbi_image_free(data);
        FS_INFO("Builded ImageData from Texture!");
        return true;
    }

    bool TextureManager::loadCube(Task &task) {
        auto filenames = std::get<1>(task.filePath);
        int w, h, c;
        for (int i=0; i<6; i++) {
            unsigned char* data = stbi_load(filenames[i].c_str(), &w, &h, &c, 0);
            if (!data) {
                FS_CORE_ERROR("Failed to load image: {0}", filenames[i]);
                return false;
            }
            size_t dataSize = w * h * c;
            auto* filedatas = std::get_if<1>(&task.fileData);
            (*filedatas)[i].assign(data, data + dataSize);
            stbi_image_free(data);
        }
        task.width = w, task.height = h, task.channels = c;
        return true;
    }

    void TextureManager::loadTextureAsync(const std::variant<string, std::array<string, 6>>& name, Texture::TextureType typ, ChannelType channel, const std::function<void(TexturePtr)> &callback) {
        Task task {name, typ, channel, callback};
        {
            std::lock_guard<std::mutex> lock(mLoadQueMutex);
            mLoadQue.push(task);
            FS_INFO("Load Texture Build Task!");
        }
        mAsyncCondition.notify_one();
    }

    void TextureManager::processAsyncUploads() {
        std::queue<Task> taskToProcess;
        {
            std::lock_guard<std::mutex> lock(mUploadQueMutex);
            std::swap(taskToProcess, mUploadQue);
        }
        while(!taskToProcess.empty()) {
            FS_INFO("Build Texture from task!");
            Task task = taskToProcess.front();
            taskToProcess.pop();
            TexturePtr tex = nullptr;
            switch(task.CreateType) {
                case Texture::TextureType::Texture2D: {
                    tex = Texture::CreateTextureFromMemory(
                        std::get<0>(task.filePath),
                        std::get<0>(task.fileData).data(),
                        task.width,
                        task.height,
                        task.channel
                    );
                    break;
                }
                case Texture::TextureType::TextureCube: {
                    tex = Texture::CreateCubeTexture(
                        std::get<1>(task.filePath)[0],
                        std::get<1>(task.fileData),
                        task.width,
                        task.height,
                        task.channel
                    );
                    break;
                }
            }
            if (tex && tex->isValid()) {
                FS_INFO("Loading Texture Create Callback!");
                if (task.callback) task.callback(tex);
            }
        }
    }
    bool TextureManager::contains(const string &name) const{
        return Texture::TextureMap.contains(name);
    }

    TexturePtr TextureManager::getTexture(const string& name) {
        return Texture::TextureMap[name];
    }

    void TextureManager::remove(const string& name) {
        Texture::TextureMap.erase(name);
    }

    int TextureManager::count() const {
        return Texture::TextureMap.size();
    }

    void TextureManager::clean() {
        Texture::TextureMap.clear();
    }

}