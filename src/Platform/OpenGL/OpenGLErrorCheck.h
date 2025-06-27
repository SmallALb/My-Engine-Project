#pragma once 
//预编译宏

#define GL_ERRORCALL(func)  func;checkError(#func, __FILE__, __LINE__)


void checkError(const string& FuncName, const string& fileName, int line);