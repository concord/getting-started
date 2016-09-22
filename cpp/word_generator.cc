#include <chrono>
#include <cstdint>
#include <vector>
#include <random>
#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

static const std::vector<std::string> kOurFamousLines = {
  "How", "do", "I", "love", "thee", "Let", "me", "count"};

class WordGenerator final : public bolt::Computation {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;

  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing word generator [cpp]";
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void destroy() override {
    LOG(INFO) << "Destructing word generator [cpp]";
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    for(auto i = 0u; i < 10000; ++i) {
      std::string bin = kOurFamousLines[i % kOurFamousLines.size()];
      ctx->produceRecord("words", bin, "");
    }
    ctx->setTimer(key, bolt::timeNowMilli());
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "word-generator";
    m.ostreams.insert("words");
    return m;
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {}
};

int main(int argc, char *argv[]) {
  // int i = 0; while(!i){sleep(1);} // gdb hack
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<WordGenerator>(), argc, argv);
  return 0;
}
