// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-30 19:28:20
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 22/22 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 56 milli secs
 * Progress: 198/198 test cases passed.
 *
 * */

#include "base/public/common_head.h"

// http://discuss.leetcode.com/questions/287/best-time-to-buy-and-sell-stock-iii
namespace NB {
int MaxProfit(std::vector<int> & v) {
  if (v.size() == 0) return 0;
  std::vector<int> forward(v.size(), 0);
  int min_idx = 0;
  for (int i = 1; i < v.size(); i++) {
    if (v[i] < v[min_idx]) min_idx = i;
    forward[i] = std::max(forward[i - 1], v[i] - v[min_idx]);
  }
  // LOG(INFO) << JoinVector(forward);
  
  int max_idx = v.size() - 1;
  int backward = 0;
  int rs = forward[v.size() - 1];
  for (int i = v.size() - 2; i >= 0; i--) {
    if (v[i] > v[max_idx]) max_idx = i;
    if (v[max_idx] - v[i] > backward) {
      backward = v[max_idx] - v[i];
      if (i != 0 && forward[i - 1] + backward > rs) rs = forward[i - 1] + backward;
    } else {
      if (forward[i] + backward > rs) rs = forward[i] + backward;
    }
  }
  return rs;
}
}  // namespace NB


namespace algorithm {

int DP(std::vector<int> & v, int * idx = NULL) {
  if (v.size() <= 1) return 0;
  std::vector<int> dp(v.size(), 0);
  dp[1] = v[1] - v[0];
  int max = dp[1];
  int j = 1;

  for (int i = 2; i < dp.size(); i++) {
    int t = v[i] - v[i - 1];
    if (t > dp[i- 1] + t) dp[i] = t;
    else dp[i] = dp[i - 1] + t;
    if (dp[i] > max) {
      max = dp[i];
      j = i;
    }
  }
  if (idx != NULL) *idx = j;
 return max > 0 ? max : 0;
}

/*
 * 最大的两个不可能是这样
 *         max
 *     -----------
 *   -----    --------
 *    max1       max2
 * */


int MaxProfitNew(std::vector<int> & v) {
  if (v.size() <= 1) return 0;
  int end = 0;
  int max1 = DP(v, &end);

  int tsum = 0;
  int begin = end;
  while (tsum != max1) {
    tsum += v[begin] - v[begin - 1];
    begin--;
  }

  std::vector<int> tmp;
  tmp.insert(tmp.end(), v.begin(), v.begin() + begin);
  int max2 = DP(tmp);
  tmp.clear();
  tmp.insert(tmp.end(), v.begin() + end + 1, v.end());
  max2 = std::max(max2, DP(tmp));
  max2 += max1;

  int max3 = 0;
  int i = begin + 1;
  while(i <= end) {
    while (v[i] >= v[i - 1] && i <= end) i++;
    if (i > end) break;
    int foo = v[i - 1] - v[begin];
    while(v[i] < v[i - 1] && i <= end) i++;
    max3 = std::max(foo + (v[end] - v[i - 1]), max3);
  }
  return std::max(max2, max3);
}

// 会 T
int MaxProfit(std::vector<int> & v) {
  if (v.size() <= 1) return 0;
  int rs = 0;

  std::vector<int> p1;
  std::vector<int> p2;
  for (int i = 0; i < v.size(); i++) {
    p1.clear();
    p2.clear();
    for (int k = 0; k <= i; k++) p1.push_back(v[k]);
    for (int k = i + 1; k < v.size(); k++) p2.push_back(v[k]);
    rs = std::max(DP(p1) + DP(p2), rs);
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  /*
  foo.push_back(1);
  foo.push_back(-1);
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(5);
  foo.push_back(3);
  foo.push_back(6);
  foo.push_back(10);
  foo.push_back(1);
  foo.push_back(1);
  foo.push_back(1);
  */
  LOG(INFO) << MaxProfitNew(foo);
  LOG(INFO) << NB::MaxProfit(foo);


  return 0;
}