#pragma once

#include <string>
#include <vector>

struct PosPair{
  long long begin{-1};
  long long end{-1};
};

PosPair StrCMPFirst(const std::string& Main, const std::string& Match) {
  if (Match.empty()) return {-1, -1};
  std::vector<size_t> Pi(Match.size());
  long long len = 0;
  for (size_t i = 0; i < Match.size(); i++) {
    while(len && Match[i] != Match[len]) len = Pi[len - 1];
    if (Match[i] == Match[len]) Pi[i] = ++len;
  }

  for (size_t i = 1, len = Pi[i-1]; i<Main.size(); i++) {
    while(len && Main[i] != Main[len]) len = Pi[len - 1];
    if (Main[len] == Main[i]) len++;
    if (len == Match.size()) return {long long(i - Match.size() + 1), long long(i)};
  }

  return {-1, -1};
}
