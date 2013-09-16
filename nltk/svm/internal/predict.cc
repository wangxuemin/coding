// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 13:16:12
// File  : predict.cc
// Brief :
#include "../public/predict.h"
#include "../public/scale.h"
namespace nltk {
namespace svm {

void Predict::LoadModel(const std::string path) {
  Model::GetInstance().LoadModel(path);   
  Kernel::GetInstance().Set(Model::GetInstance().para_);
}

void Predict::SvmPredict(const std::string input,
                         const std::string output) {
  std::string rs = "";    
  std::string content;
  std::vector<std::string> lines;
  file::File::ReadFileToStringOrDie(input, &content);
  Tokenize(content, "\r\n", &lines);
  std::vector<std::string> parts;
  for (int i = 0; i < lines.size(); i++) {
    ProblemNode node;
    parts.clear();
    Tokenize(lines[i], "\t ", &parts);
    CHECK_GT(parts.size(), 0);
    for (int j = 0; j < parts.size() ; j++) {
      std::vector<std::string> foo;
      SplitString(parts[j], ':', &foo);
      CHECK_EQ(foo.size(), 2);
      int32_t index = StringToInt(foo[0]);
      double value = StringToDouble(foo[1]);
      CHECK_GE(index, 0);
      // scale
      value = MaxMinScale::GetInstance()->Do(index, value,
              &(Model::GetInstance().feature_max_min_));
      node.element.insert(index, value);
    }
    rs += (IntToString(SvmPredict(node)) + "\n");
  }
  file::File::WriteStringToFile(rs, output);
}

int32_t Predict::SvmPredict(ProblemNode & input) {
  input.LogContent();
  std::map<int32_t, int32_t> votes;
  int32_t max_vote = 0;
  int32_t max_lable = 0;
  for (std::map<int32_t, int32_t>::reverse_iterator i =
       Model::GetInstance().start_.rbegin();
       i != Model::GetInstance().start_.rend(); i++) {
    std::map<int32_t, int32_t>::reverse_iterator tmp = i;
    tmp++;
    for (std::map<int32_t, int32_t>::reverse_iterator j = tmp;
         j != Model::GetInstance().start_.rend(); j++) {
      base::shared_ptr<ModelNode> foo =
            (*Model::GetInstance().model_[i->first].get())[j->first];
      CHECK(foo.get() != NULL);
      double bar = 0;
      CHECK(Model::GetInstance().count_.count(i->first));
      for (int k = 0; k < Model::GetInstance().count_[i->first]; k++) {
        double alpha;
        CHECK(foo->alpha.get(k, &alpha));
        if (!Free(alpha * +1, i->first)) {
          VLOG(3) << "bound alpha:" << alpha << " at:" << k;
          continue;
        }
        VLOG(3) << i->second << " " << k;
        VLOG(3) << "alpha:" << alpha << "kernel:" << Kernel::GetInstance().
                 Do(*Model::GetInstance().node_[j->second + k].get(), input);
        bar += alpha * Kernel::GetInstance().
               Do(*Model::GetInstance().node_[i->second + k].get(), input);
        VLOG(3) << "bar:" << bar;
      }
      CHECK(Model::GetInstance().count_.count(j->first));
      for (int k = 0; k < Model::GetInstance().count_[j->first]; k++) {
        double alpha = 0;
        CHECK(foo->alpha.get(Model::GetInstance().count_[i->first] + k, &alpha));
        if (!Free(alpha * -1, j->first)) {
          VLOG(3) << "bound alpha:" << alpha << " at:" << k;
          continue;
        }
        VLOG(3) << j->second << " " << k << " size:"
                << Model::GetInstance().node_.size();
        VLOG(3) << "alpha:" << alpha << "kernel:" << Kernel::GetInstance().
                 Do(*Model::GetInstance().node_[j->second + k].get(), input);
        bar += alpha * Kernel::GetInstance().
               Do(*Model::GetInstance().node_[j->second + k].get(), input);
        VLOG(3) << "bar:" << bar;
      }
      bar -= foo->b;
      VLOG(3) << "bar:" << bar << " -b: " << foo->b;
      if (bar > 0)  {
        if (votes.count(i->first)) {
          votes[i->first]++;
        } else {
          votes[i->first] = 1;
        }
        if (max_vote < votes[i->first]) {
          max_vote = votes[i->first];
          max_lable = i->first;
        }
      } else {
        if (votes.count(j->first)) votes[j->first]++;
        else votes[j->first] = 1;
        if (max_vote < votes[j->first]) {
          max_vote = votes[j->first];
          max_lable = j->first;
        }
      }
    }
  }
  VLOG(3)<< JoinKeysValues(&votes);
  return max_lable;
}
}  // namespace svm
}  // namespace nltk