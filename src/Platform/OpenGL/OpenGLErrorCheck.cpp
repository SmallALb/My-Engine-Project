#include "fspcs.h"
#include "glad/glad.h"
#include "FISH/Log.h"
#include "OpenGLErrorCheck.h"

void checkError(const string& FuncName, const string& fileName, int line) {
    GLenum errorCode = glGetError();
    string error = "";
    if (errorCode != GL_NO_ERROR) {
        switch (errorCode)
        {
        case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:  error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY: error = "OUT OF MEMORY"; break;
        default:
            error = "UNKNOWN";
            break;
        }
        FS_CORE_ERROR("(line{0})GLERROR in file \"{1}\" GLfunction_{2}, {3}", line, fileName, FuncName.c_str(), error.c_str());

    }
}