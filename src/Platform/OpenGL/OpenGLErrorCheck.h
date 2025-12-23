#pragma once 
// 定义两个基础宏
#define GL_CALL_1(func) \
  func; \
  (void)checkError(#func, __FILE__, __LINE__)


#define GL_CALL_2(func, result) \
  func; \
  result = checkError(#func, __FILE__, __LINE__)


#define GL_ERRORCALL_HELPER(_1, _2, NAME, ...) NAME
#define GL_ERRORCALL(...) GL_ERRORCALL_HELPER(__VA_ARGS__, GL_CALL_2, GL_CALL_1)(__VA_ARGS__)


bool checkError(const string& FuncName, const string& fileName, int line);