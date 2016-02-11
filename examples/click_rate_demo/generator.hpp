#include <random>

#include "gen-cpp/click_constants.h"
#include "gen-cpp/click_types.h"

class Generator {
  public:
  Generator() {
    rand_.seed(std::random_device()());
  }
  
  protected:
  std::string randomPublisher() {
    const auto randPub =
      dist_(rand_) % thrift::g_click_constants.PUBLISHER_TO_STRING.size();
    return thrift::g_click_constants.PUBLISHER_TO_STRING.at(randPub);
  }
  uint64_t randomImpression() { return dist_(rand_); }
  uint64_t numberOfClicks() { return dist_(rand_) % 50; }

  private:
  std::mt19937 rand_;
  std::uniform_int_distribution<uint64_t> dist_;
};
