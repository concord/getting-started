#pragma once
#include <random>
#include "ctr_utils.hpp"

class Generator {
  public:
  Generator() {
    rand_.seed(std::random_device()());
  }
  
  protected:
  std::string randomPublisher() {
    auto iterator = string_to_publisher.begin();
    std::advance(iterator, (dist_(rand_) % string_to_publisher.size()));
    return iterator->first;
  }
  uint64_t randomImpression() { return dist_(rand_); }
  uint64_t numberOfClicks() { return dist_(rand_) % 50; }

  private:
  std::mt19937 rand_;
  std::uniform_int_distribution<uint64_t> dist_;
};
