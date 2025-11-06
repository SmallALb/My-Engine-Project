#include "fspcs.h"
#include "FISH/Object/Object.h"
#include "ModelImporter.h"

namespace FISH {
    std::shared_ptr<ModelImporter> ModelImporter::CreateImporter(ModelType type) {
    return std::shared_ptr<ModelImporter>();
    }
}