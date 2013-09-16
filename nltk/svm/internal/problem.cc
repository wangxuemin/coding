// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 00:15:29
// File  : problem.cc
// Brief :

#include "../public/problem.h"
#include "../public/scale.h"

namespace nltk {
namespace svm {

DEFINE_bool(scale, true, "");

bool Problem::ReadFile(std::string path) {
  std::string content;
  std::vector<std::string> lines;
  file::File::ReadFileToStringOrDie(path, &content);
  Tokenize(content, "\r\n", &lines);
  std::vector<std::string> parts;
  double max_feature_num = 0;
  for (int i = 0; i < lines.size(); i++) {
    ProblemNode * node = new ProblemNode();
    node->line_no = i;
    parts.clear();
    Tokenize(lines[i], "\t ", &parts);
    CHECK_GT(parts.size(), 1);
    CHECK(parts[0].find(':') == std::string::npos);
    node->lable = StringToInt(parts[0]);

    for (int j = 1; j < parts.size() ; j++) {
      std::vector<std::string> foo;
      SplitString(parts[j], ':', &foo);
      CHECK_EQ(foo.size(), 2);
      int32_t index = StringToInt(foo[0]);
      double value = StringToDouble(foo[1]);
      CHECK_GE(index, 0);
      node->element.insert(index, value);
      if (index > max_feature_num) max_feature_num = index;
    }
    base::shared_ptr<ProblemNode> foo;
    foo.reset(node);
    node_.push_back(foo);
  }
  sort(node_.begin(), node_.end(), Cmp);
  int32_t before = 0;
  for (int i = 0; i < node_.size(); i++) {
    if (i == 0 || node_[i]->lable != before) {
      start_.insert(std::make_pair(node_[i]->lable, i));
      before = node_[i]->lable;
      count_.insert(std::make_pair(node_[i]->lable, 1));
    } else {
      count_[before]++;
      before = node_[i]->lable;
    }
  }
  FLAGS_gamma = 1.0 / static_cast<double>(max_feature_num);
  para_.reset(new Parameter());
  if (FLAGS_scale) {
    LOG(INFO) << "scaling begin...";
    MaxMinScale::GetInstance()->Do(&node_);
  }
  LogContent();
  return true;
}
}  // namespace svm
}  // namespace nltk