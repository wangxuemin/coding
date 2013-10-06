// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-23 10:11:08
// File  : code.cc
// Brief :

#include <algorithm>
#include <map>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 67/67 test cases passed.
 *
 * */

namespace algorithm {

const char * BM(const char * str, const char * pattern) {
  int p_len = strlen(pattern);
  int s_len = strlen(str);
  if (p_len == 0) return str;
  
  std::map<char, int> bc;
  std::vector<int> gs;
  std::vector<int> suffix;

  for (int i = 0; i < p_len; i++) {
    if (bc.count(pattern[i])) bc[pattern[i]] = i;
    else bc.insert(std::make_pair(pattern[i], i));
  }

  suffix.resize(p_len);
  gs.resize(p_len);

  suffix[p_len - 1] = p_len;
  for (int i = p_len - 2; i >= 0; i--) {
    int k = 0;
    while (pattern[i - k] == pattern[p_len - 1 - k]) {
      k++;
    }
    suffix[i] = k;
  }

  for (int i = 0; i < p_len; i++) gs[i] = p_len;

  for (int i = p_len - 2; i >= 0; i --) {
    if (suffix[i] == i + 1) {
      for (int j = p_len - 1 - (i + 1); j >= 0; j--) {
        gs[j] = p_len - 1 - i;
      }
      break;
    }
  }

  for (int i = 0; i < p_len - 1; i++) {
    gs[p_len - 1 - suffix[i]] = p_len - 1 - i;
  }

  int k = 0;
  while (k <= s_len - p_len) {
    int m = 0;
    while (m < p_len &&
           str[k + p_len - 1 - m] == pattern[p_len - 1 - m]) {
      m++;
    }
    if (m == p_len) return &str[k];
    int foo = p_len - m;
    if (bc.count(str[k + p_len - 1 - m])) {
      foo = p_len - 1 -m - bc[str[k + p_len - 1 - m]];
    }
    int move = std::max(gs[p_len - 1 - m], foo);
    k += move;
    
    // LOG(INFO) << "foo:" << foo << " gs:" << gs[p_len - 1 - m] << " move:" << move
    //           << " k :" << k;
    // LOG(INFO) << "move to char:" << str[k];
  }
  return NULL;
}

}  // namespace algorithm

namespace algorithm {

/*
 * 标准写法，在已经知道的当前ｉ计算 i + 1的next
 *
 * 以后要使用这个，更漂亮
 * i  s  s  i  p
 * -1 0  0  -1
 *  此时i = 3 但j = ０,所以求解的ｐ的next是正确的
 * */

void Next(const char * pattern, std::vector<int> & next) {
  int size = strlen(pattern);
  next[0] = -1;
  if (size > 1) {
    int k = next[0];
    int i = 0;
    while (i < size) {
      while (k >=0 && pattern[k] != pattern[i]){
        k = next[k];
      }
      k++;
      i++;
      if (pattern[i] == pattern[k]) next[i] = next[k];
      else next[i] = k;
    }
  }
  LOG(INFO) << "next:" << JoinVector(next);
}

const char * KMP(const char * str, const char * pattern) {
  int p_len = strlen(pattern);
  int s_len = strlen(str);
  if (p_len == 0) return str;

  std::vector<int> next;
  next.resize(p_len);

/*
 * 自己常用的这种求解next数组的方式不能使用优化方案
 * 比如 pattern :i  s  s  i  p
 *              -1  0  0  -1 
 * 由于ｉ的next为-1　求解ｐ的next时出错
 * */
/*
  next[0] = -1;
  if (p_len > 1) {
    next[1] = 0;
    for (int i = 2; i < p_len; i++) {
      int k = next[i - 1];
      // 在有优化的条件下第一次的str[k] 一定不等于 str[i - 1]
      // LOG(INFO) << pattern[k] << " " << pattern[i - 1];
      while (k >= 0 && pattern[k] != pattern[i - 1]) {
        k = next[k];
      }
      k++;
      next[i] = k;
    }
  }
*/
  // 标准求解next的方法
  Next(pattern, next);
  int k = 0;
  int j = 0;
  while (k <= s_len - p_len) {
    while (j < p_len && str[k] == pattern[j]) {
      j++;
      k++;
    }
    if (j == p_len) return &str[k - j];
    j = next[j];
    if (j < 0) {
      j++;
      k++;
    }
  }
  return NULL;
}

}

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "aaa";
  std::string pattern = "aaa";
  if (BM(str.c_str(), pattern.c_str()) != NULL) {
    LOG(INFO) << "find at " << BM(str.c_str(), pattern.c_str());
  } else {
    LOG(INFO) << "not find";
  }

  if (KMP(str.c_str(), pattern.c_str()) != NULL) {
    LOG(INFO) << "KMP find at " << KMP(str.c_str(), pattern.c_str());
  } else {
    LOG(INFO) << "KMP not find";
  }
  return 0;
}