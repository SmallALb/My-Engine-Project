#pragma once

#include "ModelImporter.h"

namespace FISH {
    class API_ ModelManager {
        ModelManager();
    public:

        ~ModelManager();

        void loadModelAsync(const string& path, ModelType type, std::function<void(Object3DPtr)> func);
        
        void processAsyncUploads();

        Object3DPtr getModel(const string& path);

        
        bool contains(const string& path) const;

        int count() const;

        void clean();
        
        static ModelManager& get();

    private:
        struct Task {
            string path;
            ModelType type;
            std::function<void(Object3DPtr)> callback;
            ModelData data;
        };

        void asyncLoadToMemory();

        Object3DPtr loadData(Task& task);
    private:
        mutable std::mutex mLoadQueMutex;
        mutable std::mutex mUploadQueMutex;
        std::queue<Task> mLoadQue;
        std::queue<Task> mUploadQue;
        std::condition_variable mAsyncCondition;
        std::thread mAsyncLoadThread;
        std::atomic<bool> mAsyncThreadRunning;
    };


}
