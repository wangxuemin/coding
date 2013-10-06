// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 16:56:25
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 11/11 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 34/34 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

std::vector<int> GetRow(int idx) {
  std::vector<std::vector<int> > rs(2, std::vector<int>(idx + 1, 0));
  if (idx < 0) return rs[0];
  
  rs[0][0] = 1;
  bool flag = false;
  for (int i = 2; i <= idx + 1; i++) {
    for (int j = 0; j < i; j++) {
      if (j == 0) rs[!flag][j] = rs[flag][0];
      else if (j == i - 1) rs[!flag][j] = rs[flag][j - 1];
      else rs[!flag][j] = rs[flag][j - 1] + rs[flag][j];
      // LOG(INFO) << "rs[" << !flag << "][" << j << "] :"  << rs[!flag][j];
    }
    flag = !flag;
  }
  return rs[flag];
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> rs = GetRow(2);
  LOG(INFO) << JoinVector(rs);
  return 0;
}