#pragma once

#include "FISH/Base/LockFreeQue.h"
#include <efsw/efsw.hpp>
#include "FileData/FileComponents/BaseComponent.h"

namespace FISH {
  //### Excute the File Build
  class FileBuilder {
  public:
    virtual bool excute(uint32_t FileEntity, std::stringstream& buffer) = 0;

    virtual bool afterexcute(uint32_t FileEntity) = 0;
  };

  //### File Listener
  //- Supplies function for the action trigger
  class FISHFileListener : public efsw::FileWatchListener {
  public:
    using ActionFunction = std::function<void(const string&)>;
  public:
    void addWachEntity(uint32_t entity);

    void setActionToFunction(uint32_t entity, efsw::Action action, const ActionFunction& func);

    void setTirgger(const std::function<bool()>& tfunc);    

     void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "") override;
  private:
    //FileEntity with Action to tirgger
    std::unordered_map<string, std::array<ActionFunction, 5>> mActionsFunction;
    std::unordered_set<string> mListenerFilePaths;
    //tirgger function
    std::function<bool()> mTirgger{nullptr};
  };

  //### File System
  class FileSystem : public FISH_System {
    FileSystem() {}
  public:
    void OnAttach() override;

    void OnUpdate(float dt) override;

    void OnDetach() override;

    void AsyncUpdate() override;

    static FileSystem* GetInstance();

    uint32_t creatFileEntity(const string& filepath, FileType type, FileBuilder* builder = nullptr);

    void watchFile(uint32_t entity);

    void setWatchActionFunc(uint32_t entity, efsw::Action action, const FISHFileListener::ActionFunction& func);

    void stopWatchFile(uint32_t entity);

    void destoryFile(uint32_t entity);

    bool checkReload() ;

    void setLimitTime(float t);

    void reLoadFile(uint32_t entity, FileBuilder* builder = nullptr);

    string getFileName(uint32_t entity) const;

    string getFilePath(uint32_t entity) const;

    string getFileContext(uint32_t entity) const;
  private:
    bool readFromMemory(std::pair<uint32_t, FileBuilder*>& task);
  private: 
    float accTime{0.0f};
    float limitTime{0.5f};
    LockFreeQue<std::pair<uint32_t, FileBuilder*>> mNeedLoadDataQue;
    LockFreeQue<std::pair<uint32_t, FileBuilder*>> mUpLoadDataQue;
    static FileSystem* ptr;
    efsw::FileWatcher* mWatcher{nullptr};
    FISHFileListener* mListener{nullptr};

  };


}