#include "fspcs.h"
#include "FISH/System.h"
#include "../D/ShaderData.h"
#include "../C/ShaderComponent.h"
#include "ShaderSystem.h"
#include "FISH/FileSystem.h"
#include "FISH/Log.h"
#include <re2/re2.h>
#include "../GPU/ShaderCreator.h"

namespace FISH {

  //### Shader Builder
  class ShaderFileBuilder : public FileBuilder {
    //Regx
    inline static RE2 VERSION_REGEX{R"(^\s*#version\s+(\d+))"};
    // 
    inline static RE2 STRUCT_REGEX{R"(struct\s+(\w+)\s*\{)"}; 

    inline static RE2 BINDING_REGEX{R"(binding\s*=\s*(\d+))"};
    inline static RE2 LOCATION_REGEX{R"(location\s*=\s*(\d+))"};
    inline static RE2 SET_REGEX{R"(set\s*=\s*(\d+))"};
    //#### note find
    inline static RE2 SINGLE_LINE_COMMENT_REGEX{R"(^\s*//.*$)"};
    inline static RE2 CLEAN_COMMENTS_REGEX{R"((?:/\*.*?\*/|//.*))"};
    //#### struct member find
    inline static RE2 STRUCT_MEMBER_REGEX{R"((\w+)\s+(\w+)(?:\[(\d+)\])?\s*;)"};
    //### function find
    inline static RE2 FUNC_REGEX{R"((\w+)\s+\w+\s*\(.*\)\s*\{)"};

    inline static RE2 VAR_DECL_REGEX{
      R"(^\s*(?:layout\s*\(\s*([^)]+)\s*\)\s*)?)"  // layout部分（可选）
      R"((uniform|in|out|attribute|varying|buffer|const)?\s*)"  // 限定符（可选，const需要特殊处理）
      R"((\w+(?:\s*<\s*\w+\s*>)?)\s+)"  // 类型（必需）
      R"((\w+))"  // 名称（必需）
      R"((?:\s*\[(\d*)\])?)"  // 数组大小（可选）
      R"(\s*;)"  // 分号结束
    };

    inline static RE2 CONST_DECL_REGEX{
      R"(^\s*(?:layout\s*\(\s*([^)]+)\s*\)\s*)?)"  // layout部分（可选）
      R"(const\s+)"  // const限定符
      R"((\w+(?:\s*<\s*\w+\s*>)?)\s+)"  // 类型
      R"((\w+))"  // 名称
      R"(\s*=\s*[^;]+)"  // 等号和值
      R"(\s*;)"  // 分号结束
    };

  public:
    ShaderFileBuilder(uint32_t entity, Registry& registry) : ShaderEntity(entity), mRegistry(registry) {}

    bool excute(uint32_t FileEntity, std::stringstream& buffer) override {
      buffer.seekg(0);
      auto& Info = mRegistry.get<ShaderInfo>(ShaderEntity);
      FS_CORE_INFO("Try to Build ShaderFile!");
      string cleanC = cleanShaderCodeNoteAndFunction(buffer.str());
      FS_CORE_INFO("CORE: \n{}", cleanC);
      //Get Version
      string version;
      if (RE2::PartialMatch(cleanC, VERSION_REGEX, &version)) Info.Version = stoull(version);

      std::istringstream stream(cleanC);
      string line;
      int lines = 0;
      string currentStuctName;

      while(std::getline(stream, line)) {
        // struct
        lines++;
        string structName;
        if (RE2::PartialMatch(line, STRUCT_REGEX, &structName)) {
          currentStuctName = structName;
          Info.ShaderStructs.emplace(currentStuctName, ShaderStructDescribe());
          continue;
        }

        if (!currentStuctName.empty()) {
          if (line.find('{') != string::npos) continue;
          parseStructMember(Info, currentStuctName, line);
          //end the struct
          if (line.find("}") != string::npos) currentStuctName.clear();
          continue;
        }

        parseVariableDeclaration(Info, line);
      }
      FS_CORE_INFO("End Build currentFile lines: {}!", lines);

      return true;
    }


    bool afterexcute(uint32_t FileEntity) override {
      return ShaderSystem::GetInstance()->buildShader(ShaderEntity, FileEntity);
    }
  private:
    static std::string trim(const string& str) {
      size_t first = str.find_first_not_of(" \t\r\n");
      if (first == string::npos) return "";
      size_t lst = str.find_last_not_of(" \t\r\n");
      return str.substr(first, lst-first + 1);
    }

    static std::string changeLine(const string& str) {
      FS_INFO("clean char: {}", str.back());
      if (str.back() == ';' || str.back() == '{' || str.back() == '}', str.back() == ',') return "\n";
      else return "";
    }

    shaderDataType stringToShaderDataType(const string& typeStr) {
        static const std::unordered_map<std::string, shaderDataType> typeMap = {
            {"float", shaderDataType::FLOAT},
            {"int", shaderDataType::INT},
            {"uint", shaderDataType::UINT},
            {"bool", shaderDataType::BOOL},
            {"vec2", shaderDataType::VEC2},
            {"vec3", shaderDataType::VEC3},
            {"vec4", shaderDataType::VEC4},
            {"ivec2", shaderDataType::IVEC2},
            {"ivec3", shaderDataType::IVEC3},
            {"ivec4", shaderDataType::IVEC4},
            {"uvec2", shaderDataType::UVEC2},
            {"uvec3", shaderDataType::UVEC3},
            {"uvec4", shaderDataType::UVEC4},
            {"bvec2", shaderDataType::BVEC2},
            {"bvec3", shaderDataType::BVEC3},
            {"bvec4", shaderDataType::BVEC4},
            {"mat3", shaderDataType::MAT3},
            {"mat4", shaderDataType::MAT4},
            {"sampler1D", shaderDataType::SAMPLE1D},
            {"sampler2D", shaderDataType::SAMPLE2D},
            {"sampler3D", shaderDataType::SAMPLE3D},
            {"samplerCube", shaderDataType::SAMPLECUBE},
            {"sampler1DArray", shaderDataType::SAMPLE1DARRAY},
            {"sampler2DArray", shaderDataType::SAMPLE2DARRAY},
            {"samplerCubeArray", shaderDataType::SAMPLECUBEARRAY}
        };
        
        auto it = typeMap.find(typeStr);
        if (it != typeMap.end()) {
            return it->second;
        }
        return shaderDataType::STRUCT; // 默认为STRUCT类型
    }
    

    ShaderDataTypeAttribute getVariableAttribute(const string& qualifier) {
      if (qualifier.empty()) return ShaderDataTypeAttribute::NONE;
      if (qualifier == "uniform") return ShaderDataTypeAttribute::UNIFORM;
      if (qualifier == "in") return ShaderDataTypeAttribute::INTYP;
      if (qualifier == "out") return ShaderDataTypeAttribute::OUTTYP;
      if (qualifier == "inout") return ShaderDataTypeAttribute::INOUT;
      if (qualifier == "attribute") return ShaderDataTypeAttribute::ATTRIBUTE;
      if (qualifier == "varying") return ShaderDataTypeAttribute::VARYING;
      if (qualifier == "const") return ShaderDataTypeAttribute::CONSTTYP;
      if (qualifier == "buffer") return ShaderDataTypeAttribute::SET;
      return ShaderDataTypeAttribute::NONE;
    }

    //clean the note and function
    string cleanShaderCodeNoteAndFunction(const string& code) {
      auto cleanC = code;
      //Remove note block
      RE2::GlobalReplace(&cleanC, CLEAN_COMMENTS_REGEX, "");
      FS_INFO("Clean comments");
      std::istringstream ss(cleanC);
      std::ostringstream sso;
      string line;
      bool infunctionBlock{0};
      bool inStructBlock{0};
      std::vector<string> lines;
      int BlockDepth{0};
      
      //Read All
      while(std::getline(ss, line)) lines.push_back(line); 
      
      for (size_t i = 0; i<lines.size(); i++) {
        //Note Remove
        size_t pos = lines[i].find("//");
        if (pos != string::npos) lines[i] = lines[i].substr(0, pos);
        lines[i] = trim(lines[i]);
        
        //Struct
        if (!infunctionBlock && !inStructBlock) {
          string structName;
          if (RE2::PartialMatch(lines[i], STRUCT_REGEX, &structName)) {
            inStructBlock = 1;
            sso<< lines[i] << "\n";
            BlockDepth = 1;
            continue;
          }
        }

        //Function
        if (!infunctionBlock && !inStructBlock) {
          bool isFunc = RE2::PartialMatch(lines[i], FUNC_REGEX);
          if (!isFunc && i+1 < lines.size()) {
            isFunc = 
              lines[i].find(')') != string::npos && 
              trim(lines[i+1]).contains('{') &&
              !RE2::PartialMatch(lines[i], STRUCT_REGEX);
          }

          if (isFunc) {
            BlockDepth = 1;
            infunctionBlock = true; continue;
          }
        }


        //if in Function or Struct Block
        if (infunctionBlock || inStructBlock) {
          for (auto& c : lines[i]) switch(c) {
            case '{': {
              infunctionBlock = BlockDepth == 0;
              BlockDepth++; break;
            }
            case '}': {
              BlockDepth--;
              BlockDepth = BlockDepth<0 ? 0: BlockDepth;
              if (BlockDepth == 0) {
                if (inStructBlock) {
                  sso<< lines[i] <<"\n";
                  inStructBlock = false;
                }
                infunctionBlock = 0;
                continue;
              }
              break; 
            }
          }
          if (inStructBlock) {
            sso << lines[i] << "\n";
          }
          continue;
        }

        if (!infunctionBlock && !BlockDepth && !lines[i].empty())
         sso << lines[i] << "\n";
      }
      FS_INFO("Shader Clean Done!");


      return sso.str();
    }

    void parseStructMember(ShaderInfo& Info, const string& structname, const string& line) {
        string typeStr, name;
        string arraySizeStr;
        auto& Des = Info.ShaderStructs[structname];

        if (RE2::FullMatch(line, STRUCT_MEMBER_REGEX, &typeStr, &name, &arraySizeStr)) {
          ShaderStructMember member;
          member.name = name;
          member.type = stringToShaderDataType(typeStr);
          member.arraySize = arraySizeStr.empty() ? 0 : std::stoul(arraySizeStr);
          if (member.type == shaderDataType::STRUCT) member.structName = typeStr; 
          Des.structInner.push_back(member);
          FS_CORE_INFO("\nStruct Name: {}, \n \t{} {}", structname, typeStr,  member.name);
        }

    }

    void parseVariableDeclaration(ShaderInfo& Info, const string& line) {
      string layoutStr, qulifier, typeStr, name, arraySizeStr;

      if (RE2::FullMatch(line, CONST_DECL_REGEX, &layoutStr, &typeStr, &name)) {
        createShaderDataDescribe(Info, "const", typeStr, name, "", layoutStr);
        return;
      }
      if (RE2::FullMatch(line, VAR_DECL_REGEX, &layoutStr, &qulifier, &typeStr, &name, & arraySizeStr)) {
        createShaderDataDescribe(Info, qulifier, typeStr, name, arraySizeStr, layoutStr);
      }
      
    }

    void createShaderDataDescribe(ShaderInfo& Info, const string& qualifier, const string& typeStr, const string& name, const string& arraySizeStr, const string& layoutStr) {
      ShaderDataDescribe desc;
      desc.data_index = Info.Describes.size();
      desc.name = name;
      desc.type = stringToShaderDataType(typeStr);
      //if struct
      if (desc.type == shaderDataType::STRUCT) desc.structName = typeStr;
      //get mainAttribute
      ShaderDataTypeAttribute mainAttr = getVariableAttribute(qualifier);

      if (mainAttr != ShaderDataTypeAttribute::NONE) 
        desc.attribute[(uint8_t)mainAttr] = -1;
      //set uniform
      if (mainAttr == ShaderDataTypeAttribute::UNIFORM) {
        UniformDescribe uniformdesc{
          .data_index = Info.UniformDescribes.size()
        };
        Info.UniformDescribes.push_back(uniformdesc);
        desc.attribute[(uint8_t)mainAttr] = Info.UniformDescribes.size();
      }
      //layout
      if (!layoutStr.empty()) {
        desc.attribute[(uint8_t)ShaderDataTypeAttribute::LAYOUT] = Info.LayoutDescribes.size();
        LayoutDescribe layoutDesc{.data_index = desc.data_index};
        string value;
        //binding
        if (RE2::PartialMatch(layoutStr, BINDING_REGEX, &value)) {
          layoutDesc.binding = stoll(value);
          desc.attribute[(uint8_t)ShaderDataTypeAttribute::SET] = -1;
        }

        if (RE2::PartialMatch(layoutStr, LOCATION_REGEX, &value)) {
          layoutDesc.location = std::stoll(value);
        }
        //set layout uniform
        if (mainAttr == ShaderDataTypeAttribute::UNIFORM) {
          Info.UniformDescribes.back().HasLayout = 1;
        }        
      }

      if (!arraySizeStr.empty()) desc.size = stoull(arraySizeStr);
      
      Info.Describes.push_back(desc);

      FS_CORE_INFO("\n \tName__{}__ \n \t\tqualifier: {}, \n \t\tType: {}, Name",name, qualifier, typeStr);
      if (!arraySizeStr.empty()) FS_CORE_INFO("\n \tName__{}__\n \t\tSize For array: {}",name, arraySizeStr);
      if (!layoutStr.empty()) FS_CORE_INFO("\n \tName__{}__\n \t\t layout({})",name, layoutStr);
    }

  private:
    uint32_t ShaderEntity{0};
    Registry& mRegistry; 
  };

  ShaderSystem* ShaderSystem::ptr{nullptr};

  ShaderSystem *ShaderSystem::GetInstance() {
    if (!ptr) ptr = new ShaderSystem();
    return ptr;
  }

  uint32_t ShaderSystem::create(const string& path, ShaderType type) {
    uint32_t entity = mRegistry.create();
    mRegistry.add<ShaderInfo>(entity);
    auto& info = mRegistry.get<ShaderInfo>(entity);
    
    info.TypeOfShader = type;
    
    size_t pos = path.find_last_of("\\/");
    info.Name = pos != std::string::npos ? path.substr(pos+1) : path;
    
    auto& fsys = *FileSystem::GetInstance();
    
    uint32_t FileEntity = fsys.creatFileEntity(
      path, 
      FileType::GLSL,
      new ShaderFileBuilder(entity, mRegistry)
    );

    fsys.watchFile(FileEntity);
    
    fsys.setWatchActionFunc(FileEntity, efsw::Action::Modified, 
      [=, this](const string& file) {
        auto builder = new ShaderFileBuilder(entity, mRegistry);
        FS_CORE_INFO("Shader File Reload Tirgger!");
        FileSystem::GetInstance()->reLoadFile(FileEntity, builder);
      }
    );
    return entity;
  }

  bool ShaderSystem::buildShader(uint32_t entity, uint32_t FileEntity) {
    auto& BaseData = mRegistry.get<ShaderInfo>(entity);

    auto newHandle = std::move(ShaderCreator::CreateShader(FileSystem::GetInstance()->getFileContext(FileEntity), BaseData.TypeOfShader));
    if (!mRegistry.has<ShaderGpuHandle>(entity)) {
      if (!newHandle.alive) return false;
      mRegistry.add<ShaderGpuHandle>(entity, std::move(newHandle));
      FS_CORE_INFO("ShaderHandle had been build!");
      return true;
    }
    
    if (newHandle.alive) {
      auto& handle = mRegistry.get<ShaderGpuHandle>(entity);
      ShaderCreator::Destory(handle);
      mRegistry.add<ShaderGpuHandle>(entity, std::move(newHandle));    
      FS_CORE_INFO("ShaderHandle had been rebuild!");
      return true;
    }
    FS_CORE_ERROR("ShaderHandle rebuild failed!");
    return false;
  }
}
