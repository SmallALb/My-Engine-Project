#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Material.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/Renderer/TextureManger.h"
#include "FISH/Object/Object.h"
#include "FISH/Object/Mesh.h"
#include "ModelManager.h"

namespace FISH {
        ModelManager::ModelManager() {
            mAsyncLoadThread = std::thread(&ModelManager::asyncLoadToMemory, this);
            mAsyncThreadRunning = true;
            FS_INFO("ModelManager Builded!");
        }

        ModelManager::~ModelManager() {
            mAsyncThreadRunning = false;
            mAsyncCondition.notify_all();
            if (mAsyncLoadThread.joinable()) mAsyncLoadThread.join();
        }

        void ModelManager::loadModelAsync(const string& path, ModelType type, std::function<void(Object3DPtr)> func) {
            {
                std::lock_guard<std::mutex> lock(mLoadQueMutex);
                mLoadQue.emplace(path, type, func, ModelData());
                FS_INFO("Load Model Build Task!");
            }
            mAsyncCondition.notify_one();
        }

        void ModelManager::processAsyncUploads() {
            std::queue<Task> taskToProcess;
            {
                std::lock_guard<std::mutex> lock(mUploadQueMutex);
                std::swap(taskToProcess, mUploadQue);
            }
            while(!taskToProcess.empty()) {
                FS_INFO("Build Model from task!");
                Task task = std::move(taskToProcess.front());
                taskToProcess.pop();
                ModelImporter::ModelMap[task.path] = loadData(task);
                if (task.callback) task.callback(ModelImporter::ModelMap[task.path]);
            }
        }
        Object3DPtr ModelManager::getModel(const string &path) {
            return ModelImporter::ModelMap[path];
        }
        bool ModelManager::contains(const string &path) const {
            return ModelImporter::ModelMap.contains(path);
        }
        int ModelManager::count() const {
            return ModelImporter::ModelMap.size();
        }
        void ModelManager::clean() {
            ModelImporter::ModelMap.clear();
        }

        ModelManager &ModelManager::get() {
            static ModelManager manager;
            return manager;
        }
        void ModelManager::asyncLoadToMemory() {
            while(mAsyncThreadRunning) {
                Task task;
                {
                    FS_INFO("Waiting for Modeloads");
                    std::unique_lock<std::mutex> lock(mLoadQueMutex);
                    mAsyncCondition.wait(lock, [this]() {
                        return !mLoadQue.empty() || !mAsyncThreadRunning;
                    });
                    FS_INFO("Done! Having Model loads push in Que!");
                    if (!mAsyncThreadRunning) break;
                    if (mLoadQue.empty()) continue;

                    task = std::move(mLoadQue.front());
                    mLoadQue.pop();
                }
                task.data = ModelImporter::ImportModelData(task.path, task.type);
                if (task.data.head->valid != -1) {
                    std::lock_guard<std::mutex> lock(mUploadQueMutex);
                    mUploadQue.push(std::move(task));
                    FS_INFO("Successed To ModelData In Task!");
                }
                else FS_ERROR("Failed To ModelData In Task!");
            }
        }
        Object3DPtr ModelManager::loadData(Task &task) {
            auto& data = task.data;
            Object3DPtr head(new Object3D());

            auto func = [](auto&& self, Object3DPtr obj, const std::shared_ptr<MeshData>& meshdata)->void {
                Object3DPtr nxtobj;
                for (auto& child : meshdata->childs) {
                    switch (child->valid) {
                        case 1: {
                            MeshPtr mesh(new Mesh(child->name));
                            nxtobj = mesh;

                            //创建材质
                            MaterialPtr material(new Material());

                            VertexBufferPtr posBuffer(VertexBuffer::Create(child->positions.data(), child->positions.size() * 4));
                            VertexBufferPtr normalBuffer(VertexBuffer::Create(child->normals.data(), child->normals.size() * 4));
                            VertexBufferPtr uvBuffer(VertexBuffer::Create(child->uvs.data(), child->uvs.size() * 4));
                            VertexBufferPtr colorBuffer(VertexBuffer::Create(child->colors.data(), child->colors.size() * 4));
                            IndexBufferPtr indexBuffer(IndexBuffer::Create(child->indices.data(), child->indices.size()));

                            posBuffer->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
                            colorBuffer->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
                            normalBuffer->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});
                            uvBuffer->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});

                            //创建顶点组
                            ShapePtr shape(new Shape(posBuffer, indexBuffer));
                            shape->getVertexArrary()->AddVertexBuffer(colorBuffer);
                            shape->getVertexArrary()->AddVertexBuffer(uvBuffer);
                            shape->getVertexArrary()->AddVertexBuffer(normalBuffer);

                            material->setDiffuseColor(child->diffuseColor);

                            material->setEmissiveColor(child->emissiveColor);

                            material->setOpacity(child->opacity);
                            
                            // 根据透明度设置混合模式
                            if (material->getOpacity() < 1.0f) 
                                material->setBlendColorTag(true);

                            material->setSpecularColor(child->specularColor);
                            
                            // 高光强度
                            material->setShininess(child->shininess);

                            mesh->getMaterial() = material;
                            mesh->getShape() = shape;

                            material->setDepthWriteTag(1);
                            TextureManager::get().loadTextureAsync(
                                child->diffuseTexturePath, 
                                Texture::TextureType::Texture2D, ChannelType::RGB, 
                                [material](TexturePtr ptr) {
                                    material->setTexture(TextureType::None, ptr);
                            });

                            //清理数据
                            child->positions.clear();
                            child->uvs.clear();
                            child->normals.clear();
                            child->colors.clear();
                            child->indices.clear();

                            break;
                        }
                        case 0: {
                            nxtobj.reset(new Object3D(child->name));
                            break;
                        }
                    
                    }
                    nxtobj->setAngleX(child->rotation.x);
                    nxtobj->setAngleY(child->rotation.y);
                    nxtobj->setAngleZ(child->rotation.z);
                    nxtobj->setScale(child->scale);
                    obj->addChild(nxtobj);
                    self(self, nxtobj, child);
                }
                meshdata->childs.clear();
            };
            func(func, head, data.head);
            return head;
        }
}