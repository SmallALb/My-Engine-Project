#pragma once

#ifdef _WIN32

extern FISH::Application* FISH::CreateApplication();

//�ܺ������
int main(int argc, char** argv) {
    FISH::Log::Init();
    auto app = FISH::CreateApplication();

    app->run();
    delete app;
}


#endif