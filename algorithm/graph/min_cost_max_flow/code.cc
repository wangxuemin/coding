// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-16 22:04:08
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
static const int MAX = 1000;
int matrix[MAX][MAX];
int flow[MAX][MAX];  // flow[i][j] 表示残留网络中i --> j 还有多少流量可供流
int N;
int cost[MAX][MAX];
int pre[MAX];
int weight[MAX];

int MinCostMaxFlow(int source, int target, int * sum_cost) {
  int rs = 0;
  *sum_cost = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      flow[i][j] = matrix[i][j];
    }
  }

  while (true) {
    for (int i = 0; i < N; i++) weight[i] = INT_MAX;
    memset(pre, 0, sizeof(pre));
    std::queue<int> queue;
    queue.push(source);
    weight[source] = 0;
    while (!queue.empty()) {
      int t = queue.front();
      queue.pop();
      for (int i = 0; i < N; i++) {
        if (flow[t][i] > 0 && weight[i] > weight[t] + cost[t][i]) {
          pre[i] = t;
          queue.push(i);
          weight[i] = weight[t] + cost[t][i];
        }
      }
    }
    if (weight[target] == INT_MAX) break;
    int min_flow = INT_MAX;
    for (int i = target; i != source; i = pre[i]) {
      min_flow = std::min(min_flow, flow[pre[i]][i]);
    }
    for (int i = target; i != source; i = pre[i]) {
      flow[pre[i]][i] -= min_flow;
      flow[i][pre[i]] += min_flow;
    }
    *sum_cost += min_flow * weight[target];
    rs += min_flow;
  }
  return rs;
}


void Read(std::vector<std::vector<int> > & v, std::vector<std::vector<int> > vcost) {
  N = v.size();
  memset(matrix, 0 ,sizeof(matrix));
  memset(cost, 0, sizeof(cost));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
      if (vcost[i][j] > 0) {
        cost[i][j] = vcost[i][j];
        cost[j][i] = -vcost[i][j];
      }
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {

  return 0;
}