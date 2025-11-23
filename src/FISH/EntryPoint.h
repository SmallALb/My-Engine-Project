#pragma once

#ifdef _WIN32

extern FISH::Application* FISH::CreateApplication();

//总函数入口
int main(int argc, char** argv) {
    
    FISH::Log::Init();
    auto app = FISH::CreateApplication();

    app->run();


    FS_PROFILE_FRAME();
    delete app;
    FS_INFO("App had Deleted!");
}


#endif