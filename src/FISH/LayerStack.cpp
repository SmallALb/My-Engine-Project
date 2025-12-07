#include "fspcs.h"
#include "LayerStack.h"

namespace FISH {
    LayerStack::LayerStack() {
    }

    LayerStack::~LayerStack() {
        for (auto layer: mlayers) {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer *layer) {
        mlayers.emplace(mlayers.begin() + mlayersInsertIndex, layer);
        mlayersInsertIndex++;
    }

    void LayerStack::PushOverLay(Layer *overlay) {
        mlayers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer *layer) {
        auto it = std::find(mlayers.begin(), mlayers.end(), layer);
        if (it != mlayers.end()) {
            (*it)->OnDetach();
            mlayers.erase(it);
            mlayersInsertIndex--;
        }
    }

    void LayerStack::PopOverLay(Layer *overlay) {
        auto it = std::find(mlayers.begin(), mlayers.end(), overlay);
        if (it != mlayers.end()) {
            (*it)->OnDetach();
            mlayers.erase(it);
        }
        
    }
}
