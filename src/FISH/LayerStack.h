#pragma once

#include "Core.h"
#include "Layer.h"


namespace FISH {
    class API_ LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverLay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverLay(Layer* overlay);

        auto begin() {return mlayers.begin();}
        auto end() {return mlayers.end();}
    private:
        std::vector<Layer*>             mlayers{};
        unsigned int                    mlayersInsertIndex{0};
    };

}