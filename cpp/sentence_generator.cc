#include <chrono>
#include <cstdint>
#include <vector>
#include <random>
#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

static const std::vector<std::string> kHowDoILoveTheeLines = {
  "How do I love thee? Let me count the ways.",
  "I love thee to the depth and breadth and height",
  "My soul can reach, when feeling out of sight",
  "For the ends of being and ideal grace.",
  "I love thee to the level of every day's",
  "Most quiet need, by sun and candle-light.",
  "I love thee freely, as men strive for right.",
  "I love thee purely, as they turn from praise.",
  "I love thee with the passion put to use",
  "In my old griefs, and with my childhood's faith.",
  "I love thee with a love I seemed to lose",
  "With my lost saints. I love thee with the breath,",
  "Smiles, tears, of all my life; and, if God choose,",
  "I shall but love thee better after death."};

class SentenceGenerator final : public bolt::Computation {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;

  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing sentence generator [cpp]";
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void destroy() override {
    LOG(INFO) << "Destructing sentence generator [cpp]";
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    for(auto i = 0u; i < 10000; ++i) {
      //std::string bin = kHowDoILoveTheeLines[i % kHowDoILoveTheeLines.size()];
      ctx->produceRecord("sentences", "k", "v");
    }
    ctx->setTimer(key, bolt::timeNowMilli());
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "sentence-generator";
    m.ostreams.insert("sentences");
    return m;
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {}
};

int main(int argc, char *argv[]) {
  // int i = 0; while(!i){sleep(1);} // gdb hack
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<SentenceGenerator>(), argc,
                                 argv);
  return 0;
}
