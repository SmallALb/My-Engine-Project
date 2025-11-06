#include "fspcs.h"
#include "FISH/Log.h"
#include "FISH/Renderer/Buffer.h"
#include "FISH/Renderer/VertexArray.h"
#include "FISH/Renderer/BaseShape.h"
#include "FISH/Renderer/Texture.h"
#include "FISH/Renderer/Material.h"
#include "FISH/Object/Object.h"  
#include "FISH/Object/Mesh.h"
#include "FbxReader.h"
#include "fbxsdk.h"


namespace FISH {
    //FbxSdk接口
    class FbxReader::Impl {
    public:
        Impl() {}

        ~Impl() {
            destory();
        }

        bool initSDK() {
            if (initTag) return true;
            mManager = FbxManager::Create();
            if (!mManager) {
                FS_ERROR("unable to create FbxManager!");
                return false;
            }
            //fbx io
            FbxIOSettings* ios = FbxIOSettings::Create(mManager, IOSROOT);
            mManager->SetIOSettings(ios);

            mScenc = FbxScene::Create(mManager, "FISH scence");
            if (!mScenc) {
                FS_ERROR("unable to create FbxScenc!");
                return false;
            }

            return initTag = true;
        }

        void destory() {
            if (mScenc) {mScenc->Destroy(); mScenc = nullptr;}
            if (mImporter) {mImporter->Destroy(); mImporter = nullptr;}
            if (mManager) mManager->Destroy();
        };

        bool loadPath(const string& path) {
            int lFileMajor, lFileMinor, lFileRevision;
            int lSDKMajor, lSDKMinor, lSDKRevision;

            FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
        
            mImporter = FbxImporter::Create(mManager, "");

            if (!mImporter->Initialize(path.c_str(), -1, mManager->GetIOSettings())) {
                FS_ERROR("Init FbxImporter Failed: {}", mImporter->GetStatus().GetErrorString());
                return false;
            }

            mImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

            if (mImporter->IsFBX()) {
                FS_INFO("Fbx file Version: {}.{}.{}", lFileMajor, lFileMinor, lFileRevision);

                mManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
                mManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
                mManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
                mManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
                mManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
                mManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
            }
            bool lstate = mImporter->Import(mScenc);
            
            if (!lstate) FS_INFO("Faile to import Fbx Scenc");
            
            return lstate;
        };

        MeshPtr processToMesh(FbxNode* node) {
            FbxMesh* mesh = node->GetMesh();
            if (!mesh) return nullptr;

            FS_INFO("Processing mesh: {}", node->GetName());
            auto shape = makeShape(mesh);
            if (!shape) return FS_ERROR("Failed to Create shape from FBX mesh {}", node->GetName());

            auto Fishmesh = std::make_shared<Mesh>(shape, node->GetName());
            Fishmesh->getMaterial() = makeMaterial(node);
            Fishmesh->getMaterial()->setDepthWriteTag(1);
            FbxAMatrix lTransform = node->EvaluateGlobalTransform();
            FbxVector4 lTranslation = lTransform.GetT();
            FbxVector4 lRotation = lTransform.GetR();
            FbxVector4 lScaling = lTransform.GetS();
        
            Fishmesh->setPosition(glm::vec3(
                static_cast<float>(lTranslation[0]),
                static_cast<float>(lTranslation[1]),
                static_cast<float>(lTranslation[2])
            ));
            Fishmesh->setAngleX(static_cast<float>(lRotation[0]));
            Fishmesh->setAngleY(static_cast<float>(lRotation[1]));
            Fishmesh->setAngleZ(static_cast<float>(lRotation[2]));
            Fishmesh->setScale(glm::vec3(
                static_cast<float>(lScaling[0]),
                static_cast<float>(lScaling[1]),
                static_cast<float>(lScaling[2])
            ));
            
            FS_INFO("Successfully created mesh: {} with {} vertices", 
                    node->GetName(), shape->GetIndexCounts());
            
            return Fishmesh;
        };

        ShapePtr makeShape(FbxMesh* mesh) {
            std::vector<float>          positions;
            std::vector<unsigned int>    indexs;
            std::vector<float>          uvs;
            std::vector<float>          normals;
            std::vector<float>          colors;


            int polygonCount = mesh->GetPolygonCount();
            int controlPointsCount = mesh->GetControlPointsCount();
            FbxVector4* controlPoints = mesh->GetControlPoints();
            
            //获取法线
            FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
            
            //获取UV
            FbxGeometryElementUV* uvElement = mesh->GetElementUV();

            //多边形三角化
            for (int polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++) {
                int polygonSize = mesh->GetPolygonSize(polygonIndex);

                if (polygonSize >= 3) for (int triangleIndex = 0; triangleIndex < polygonSize - 2; triangleIndex++) {
                    int Vindexs[3] = {
                        mesh->GetPolygonVertex(polygonIndex, 0),
                        mesh->GetPolygonVertex(polygonIndex, triangleIndex + 1),
                        mesh->GetPolygonVertex(polygonIndex, triangleIndex + 2)
                    };

                    for (int i= 0; i<3; i++) {
                        int& vindex = Vindexs[i];
                        FbxVector4 position = controlPoints[vindex];
                        //位置
                        positions.push_back(position[0]);
                        positions.push_back(position[1]);
                        positions.push_back(position[2]);
                        //法线
                        FbxVector4 normal(0, 1, 0, 0);
                        if (normalElement) {
                            int normalIndex = 0;
                            switch(normalElement->GetMappingMode()) {
                                //直接就是顶点则直接取顶点下标
                                case FbxGeometryElement::eByControlPoint:
                                    normalIndex = vindex;
                                    break;
                                //多边形法线需要取顶点下标
                                case FbxGeometryElement::eByPolygonVertex:
                                    normalIndex = mesh->GetPolygonVertexIndex(polygonIndex) + i + triangleIndex * 3;
                                    break;
                                case FbxGeometryElement::eByPolygon:
                                    normalIndex = polygonIndex; // 每个多边形一个法线
                                    break;
                            }       
                            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                                if (normalIndex < normalElement->GetDirectArray().GetCount()) {
                                    normal = normalElement->GetDirectArray().GetAt(normalIndex);
                                }
                            } else if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                                if (normalIndex < normalElement->GetIndexArray().GetCount()) {
                                    int id = normalElement->GetIndexArray().GetAt(normalIndex);
                                    normal = normalElement->GetDirectArray().GetAt(id);
                                }
                            }
                        }
                        normals.push_back(normal[0]);
                        normals.push_back(normal[1]);
                        normals.push_back(normal[2]);

                        //uv
                        FbxVector2 uv(0, 0); 
                        if (uvElement) {
                            int uvIndex = 0;
                            switch(uvElement->GetMappingMode()) {
                                //直接就是顶点则直接取顶点下标
                                case FbxGeometryElement::eByControlPoint:
                                    uvIndex = vindex;
                                    break;
                                //多边形法线需要取顶点下标
                                case FbxGeometryElement::eByPolygonVertex:
                                    uvIndex = mesh->GetPolygonVertexIndex(polygonIndex) + i;
                                    break;
                                case FbxGeometryElement::eByPolygon:
                                    uvIndex = polygonIndex; // 每个多边形一个UV
                                    break;
                            }       
                            if (uvElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                                if (uvIndex < uvElement->GetDirectArray().GetCount()) {
                                    uv = uvElement->GetDirectArray().GetAt(uvIndex);
                                }
                            } else if (uvElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                                if (uvIndex < uvElement->GetIndexArray().GetCount()) {
                                    int id = uvElement->GetIndexArray().GetAt(uvIndex);
                                    uv = uvElement->GetDirectArray().GetAt(id);
                                }
                            }
                        }
                        uvs.push_back(uv[0]);
                        uvs.push_back(uv[1]);

                        colors.push_back(0.8f);
                        colors.push_back(0.8f);
                        colors.push_back(0.8f);
                        colors.push_back(1.0f);

                        indexs.push_back(indexs.size());

                    }
                }
            }

            if (positions.empty() || indexs.empty()) {
                FS_WARN("No valid geometry data found in Fbx mesh");
                return nullptr;
            }

            VertexBufferPtr posBuffer(VertexBuffer::Create(positions.data(), positions.size() * 4));
            VertexBufferPtr normalBuffer(VertexBuffer::Create(normals.data(), normals.size() * 4));
            VertexBufferPtr uvBuffer(VertexBuffer::Create(uvs.data(), uvs.size() * 4));
            VertexBufferPtr colorBuffer(VertexBuffer::Create(colors.data(), colors.size() * 4));
            IndexBufferPtr indexBuffer(IndexBuffer::Create(indexs.data(), indexs.size()));

            posBuffer->SetLayout({{ShaderDataType::Float3, VertexType::Position, "pos"}});
            colorBuffer->SetLayout({{ShaderDataType::Float4, VertexType::Color, "color"}});
            normalBuffer->SetLayout({{ShaderDataType::Float3, VertexType::Normal, "normal"}});
            uvBuffer->SetLayout({{ShaderDataType::Float2, VertexType::UV, "uv"}});

            auto shape = std::make_shared<Shape>(posBuffer, indexBuffer);
            shape->getVertexArrary()->AddVertexBuffer(colorBuffer);
            shape->getVertexArrary()->AddVertexBuffer(uvBuffer);
            shape->getVertexArrary()->AddVertexBuffer(normalBuffer);

            return shape;

        };

        MaterialPtr makeMaterial(FbxNode* node) {
            auto material = std::make_shared<Material>();
            FbxSurfaceMaterial* fbxMaterial = nullptr;
            fbxMaterial = node->GetMaterialCount() > 0 
                ? 
                    node->GetMaterial(0)
                :   
                    fbxMaterial;
            if (!fbxMaterial) return material;

            if (!fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
                auto lambert = (FbxSurfaceLambert* )fbxMaterial;

                FbxDouble3 diffuse = lambert->Diffuse.Get();
                material->setDiffuseColor(glm::vec3(
                    static_cast<float>(diffuse[0]),
                    static_cast<float>(diffuse[1]), 
                    static_cast<float>(diffuse[2])
                ));

                FbxDouble3 emissive = lambert->Emissive.Get();
                material->setEmissiveColor(glm::vec3(
                    static_cast<float>(emissive[0]),
                    static_cast<float>(emissive[1]), 
                    static_cast<float>(emissive[2])
                ));

                float opacity = static_cast<float>(lambert->TransparencyFactor.Get());
                material->setOpacity(1.0f - opacity);
                
                // 根据透明度设置混合模式
                if (material->getOpacity() < 1.0f) 
                    material->setBlendColorTag(true);
            }
            
            if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) {
                FbxSurfacePhong* phong = (FbxSurfacePhong*)fbxMaterial;
                
                // 高光颜色
                FbxDouble3 specular = phong->Specular.Get();
                material->setSpecularColor(glm::vec3(
                    static_cast<float>(specular[0]),
                    static_cast<float>(specular[1]),
                    static_cast<float>(specular[2])
                ));
                
                // 高光强度
                material->setShininess(static_cast<float>(phong->Shininess.Get()));
            }
            loadTexture(fbxMaterial, FbxSurfaceMaterial::sDiffuse, TextureType::None, material);
            return material;
        };

        void processNode(FbxNode* node, Object3DPtr fa) {
            if (!node) return;
            FbxNodeAttribute* lNodeAttribute = node->GetNodeAttribute();
            Object3DPtr nxt;
            if (lNodeAttribute) {
                FbxNodeAttribute::EType attributeType = lNodeAttribute->GetAttributeType();
                switch(attributeType) {
                    case FbxNodeAttribute::eMesh: {
                        nxt = processToMesh(node);
                        if (nxt && fa) fa->addChild(nxt);
                        break;
                    }
                    case FbxNodeAttribute::eNull: {
                        nxt = std::make_shared<Object3D>(node->GetName());
                        if(fa) fa->addChild(nxt);
                        break;
                    }
                    default:
                        FS_INFO("Skipping node Type: {}", (int)attributeType);
                }
            }
            for (int i=0; i< node->GetChildCount(); i++) processNode(node->GetChild(i), nxt);
        };


        void loadTexture(FbxSurfaceMaterial* material, const char* propertyName, TextureType textureType, MaterialPtr& outMaterial) {
            FbxProperty property = material->FindProperty(propertyName);
            if (!property.IsValid()) return;
            int textureCount = property.GetSrcObjectCount<FbxFileTexture>();
            if (textureCount > 0) {
                FbxFileTexture* texture = property.GetSrcObject<FbxFileTexture>();
                auto FishTexture = FISH::Texture::CreateTextureFromPath(texture->GetFileName());
                if (FishTexture) outMaterial->setTexture(textureType, FishTexture);
            }
        }
    public:
        FbxManager* mManager {nullptr};
        FbxScene*   mScenc {nullptr}; 
        FbxImporter*   mImporter {nullptr};
        bool          initTag{0};
    };

    FbxReader::FbxReader() {
        impl = new Impl();
    }

    FbxReader::~FbxReader() {
        delete impl;
    }



    Object3DPtr FbxReader::readModel(string path) {
        FS_INFO("Loading FBX model: {}", path);

        if (!impl->initSDK()) return FS_ERROR("Failed to Create FBXsdk");

        if (!impl->loadPath(path)) {
            impl->destory();
            return FS_ERROR("Failed to load FBX scene: {}", path);
        }


         // 创建根对象
        auto rootObject = std::make_shared<Object3D>("FBX_Root");
        
        // 处理场景根节点
        FbxNode* lRootNode = impl->mScenc->GetRootNode();
        if(lRootNode) for(int i = 0; i < lRootNode->GetChildCount(); i++) 
                impl->processNode(lRootNode->GetChild(i), rootObject);
            
        FS_INFO("FBX model loaded successfully: {} with {} children", 
                path, rootObject->getChilds().size());
        
        return rootObject;

    }

}