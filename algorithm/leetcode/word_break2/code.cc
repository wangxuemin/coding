// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 16:33:48
// File  : code.cc
// Brief :

/*
 *
 * 24 / 24 test cases passed.
 * Status: Accepted
 * Runtime: 28 ms
 * Submitted: 0 minutes ago
 *
 * */

#include "base/public/common_head.h"
#include <unordered_set>

namespace algorithm {

void DP(const std::string & s, std::unordered_set<std::string> & dict,
        std::vector<bool> & dp, int len) {
  for (int i = 0; i < s.size(); i++) {
    for (int j = i; j >= std::max(0, i - len - 1); j--) {
      const std::string & t = s.substr(j, i - j + 1);
      if (j - 1 >= 0) dp[i] = dict.count(t) ? dp[j - 1] : false;
      else dp[i] = dict.count(t) ? true : false;
      if (dp[i]) break;
    }
  }
}

void Trace(const std::string & s, std::vector<bool> & dp,
           int k, int len,
           std::vector<std::string> & path,
           std::vector<std::string> & rs,
           std::unordered_set<std::string> & dict) {
  if (k == -1) {
    std::string t = "";
    for (int i = path.size() - 1; i >= 0; i--) {
      if (!t.empty()) t.append(" ");
      t.append(path[i]);
    }
    rs.push_back(t);
  } else {
    if (dp[k] == false) return;
    for (int i = k; i >= std::max(0, k - len + 1); i--) {
      std::string t = s.substr(i, k - i + 1);
      if (dict.count(t) && ((i != 0 && dp[i - 1]) || i == 0)) {
        path.push_back(t);
        Trace(s, dp, i - 1, len, path, rs, dict);
        path.pop_back();
      }
    }
  }
}

std::vector<std::string> WordBreak(std::string & s, std::unordered_set<std::string> & dict) {
  int len = 0;
  for (std::unordered_set<std::string>::iterator i = dict.begin(); i != dict.end();
       i++) {
    len = std::max(len, (int)(i->size()));
  }
  std::vector<bool> dp(s.size(), false);
  DP(s, dict, dp, len);
  std::vector<std::string> rs;
  std::vector<std::string> path;
  Trace(s, dp, s.size() - 1, len, path, rs, dict);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string s = "catsanddog";
  std::unordered_set<std::string> dict;
  dict.insert("cat");
  dict.insert("cats");
  dict.insert("and");
  dict.insert("sand");
  dict.insert("dog");
  std::vector<std::string> rs = WordBreak(s, dict);
  LOG(INFO) << JoinVector(rs);
  return 0;
}