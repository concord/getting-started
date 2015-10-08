#include <chrono>
#include <cstdint>
#include <vector>
#include <random>
#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

// sample from D. Knuth's 5 letter words dictionary
// http://www-cs-faculty.stanford.edu/~uno/sgb.html
static const std::vector<std::string> kDictionary = {
  "beach", "stock", "hurry", "saved", "sorry", "giant", "trail", "offer"};


class WordSource final : public bolt::Computation {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;


  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing computation [cpp]";
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {

    const static auto kDictSize = kDictionary.size();

    for(auto i = 0u; i < 100000; ++i) {
      std::string bin = kDictionary[i % kDictSize];
      ctx->produceRecord("words", bin, "");
    }

    ctx->setTimer(key, bolt::timeNowMilli());
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "word-source";
    m.ostreams.insert("words");
    return m;
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {
    LOG(FATAL) << "source does not impl processRecord";
  }
};

int main(int argc, char *argv[]) {
  // int i = 0; while(!i){sleep(1);} // gdb hack
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<WordSource>(), argc, argv);
  return 0;
}
