#include "fspcs.h"
#include "System.h"
#include "FileSystem.h"
#include "FISH/Log.h"
#include <filesystem>
#include <fstream>

namespace FISH {
  FileSystem* FileSystem::ptr = nullptr;

  void FileSystem::OnAttach() {
    mWatcher = new efsw::FileWatcher();
    mListener = new FISHFileListener();
    mWatcher->followSymlinks(true);
    mWatcher->watch();
    mListener->setTirgger([this](){return checkReload();});
  }

  void FileSystem::OnUpdate(float dt) {
    accTime += dt;
    if (!mUpLoadDataQue.empty()){
      std::pair<uint32_t, FileBuilder*> FileBuildTask;
      mUpLoadDataQue.pop(FileBuildTask);
      if (FileBuildTask.second->afterexcute(FileBuildTask.first)) {
        FS_CORE_INFO("The File: {} had been Build!", FileBuildTask.first);
        delete FileBuildTask.second;
      }
    }
  }

  void FileSystem::OnDetach() {
    for (auto& [entity, state] : mRegistry.getCreatedEntitys())
      destoryFile(entity);
    delete mWatcher;
    delete mListener;
  }

  void FileSystem::AsyncUpdate() {
    std::pair<uint32_t, FileBuilder*> FileBuildTask;
    mNeedLoadDataQue.pop(FileBuildTask);
    if (readFromMemory(FileBuildTask)) mUpLoadDataQue.push(FileBuildTask);
    else if (FileBuildTask.second) delete FileBuildTask.second;
  }

  FileSystem *FileSystem::GetInstance() {
    if (!ptr) ptr = new FileSystem();
    return ptr;
  }

  uint32_t FileSystem::creatFileEntity(const string& filepath, FileType type, FileBuilder* builder) {
    auto entity = mRegistry.create();
    mRegistry.add<FileBaseData>(entity);
    mRegistry.patch<FileBaseData>(entity, [&](FileBaseData& BaseData) {
      std::filesystem::path _Path(filepath);
      string absfilepath;
      absfilepath = _Path.is_absolute() ? filepath : std::filesystem::absolute(_Path).string();
      BaseData.type = type;
      BaseData.filePath = absfilepath;
      std::filesystem::path path(BaseData.filePath);
      BaseData.fileName = path.filename().string();
    });
    FS_CORE_INFO("Create a file: {}!", entity);
    mNeedLoadDataQue.push({entity, builder});
    submit();
    return entity;
  }

  void FileSystem::watchFile(uint32_t entity) {
    FS_CORE_INFO("Try to Listen File Entity: {}", entity);
    auto& BaseData = mRegistry.get<FileBaseData>(entity);
    mListener->addWachEntity(entity);
    std::filesystem::path path(BaseData.filePath);
    BaseData.watchId = mWatcher->addWatch(path.parent_path().string(), mListener, false);

    if (BaseData.watchId < 0) {
      efsw::Error e = (efsw::Error)BaseData.watchId;
      switch(e) {
        case efsw::Error::FileNotFound: FS_CORE_ERROR("Watch File: {} Not Found", BaseData.filePath); break;
        case efsw::Error::FileNotReadable: FS_CORE_ERROR("Watch File: {} not Readable", BaseData.filePath); break;
        case efsw::Error::FileOutOfScope: FS_CORE_ERROR("Watch File: {} out of Scope", BaseData.filePath); break;
        case efsw::Error::FileRemote: FS_CORE_ERROR("Watch File: {} Remote", BaseData.filePath); break;
        case efsw::Error::FileRepeated: FS_CORE_ERROR("Watch File: {} Repeated", BaseData.filePath); break;
        case efsw::Error::WatcherFailed: FS_CORE_ERROR("Watch File: {} Watcher Failed", BaseData.filePath); break;
      }
    }
    else {
      FS_CORE_INFO("File: {} had been Listened", BaseData.filePath);
    }
  }

  void FileSystem::setWatchActionFunc(uint32_t entity, efsw::Action action, const FISHFileListener::ActionFunction& func) {
    auto& BaseData = mRegistry.get<FileBaseData>(entity);
    mListener->setActionToFunction(entity, action, func);
  }

  void FileSystem::stopWatchFile(uint32_t entity) {
    auto& BaseData = mRegistry.get<FileBaseData>(entity);
    if (!mListener) return;
    mWatcher->removeWatch(BaseData.watchId);
  }

  void FileSystem::destoryFile(uint32_t entity) {
    stopWatchFile(entity);
    mRegistry.destory(entity);
  }

  bool FileSystem::checkReload() {
    if (accTime >= limitTime) {
      accTime = 0.0f;
      return 1;
    }
    return 0; 
  }

  void FileSystem::setLimitTime(float t) {
    limitTime = t;
  }

  void FileSystem::reLoadFile(uint32_t entity, FileBuilder *builder) {
    mNeedLoadDataQue.push({entity, builder});
    submit();
  }

  string FileSystem::getFileName(uint32_t entity) const {
    const auto& BaseData = mRegistry.get<FileBaseData>(entity);
    return BaseData.fileName;
  }

  string FileSystem::getFilePath(uint32_t entity) const {
    const auto& BaseData = mRegistry.get<FileBaseData>(entity);
    return BaseData.filePath;
  }

  string FileSystem::getFileContext(uint32_t entity) const {
    auto& BaseData = mRegistry.get<FileBaseData>(entity);
    return BaseData.fileText;
  }

  bool FileSystem::readFromMemory(std::pair<uint32_t, FileBuilder*>& task) {
    auto& [entity, Builder] = task;
    auto& BaseData = mRegistry.get<FileBaseData>(entity);
    if (BaseData.filePath.empty()) {
      FS_CORE_ERROR("Error FileEntity: {} Path is empty!", entity);
      return false;
    }
    std::ifstream FileStream(BaseData.filePath);

    if (!FileStream.is_open()) {
      FS_CORE_ERROR("File: {} Cant't Open", BaseData.filePath);
      return false;
    }
    FS_CORE_INFO("FIle: {} had been Opened!", BaseData.filePath);

    std::stringstream buffer;
    buffer << FileStream.rdbuf();
    FileStream.close();
    BaseData.fileText = buffer.str();

    if (Builder) {
      return Builder->excute(entity, buffer);
    }
    return false;
  }

  void FISHFileListener::addWachEntity(uint32_t entity) {
    mListenerFilePaths.insert(FileSystem::GetInstance()->getFilePath(entity)); 
  }

  // Listener
  void FISHFileListener::setActionToFunction(uint32_t entity, efsw::Action action, const ActionFunction &func) {
    mActionsFunction[FileSystem::GetInstance()->getFilePath(entity)][action] = func;
  }

  void FISHFileListener::setTirgger(const std::function<bool()> &tfunc) {
    mTirgger = tfunc;
  }

  void FISHFileListener::handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename, efsw::Action action, std::string oldFilename) {
    auto filepath = dir + filename;
    FS_INFO("File: {} Had Changed!", filepath);
    if (mTirgger != nullptr && !mTirgger()) return;
    if (mActionsFunction.contains(filepath)) 
      mActionsFunction[filepath][action](filename);
  }
}