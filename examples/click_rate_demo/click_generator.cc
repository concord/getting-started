#include <random>
#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

#include "ctr_utils.hpp"
#include "gen-cpp/click_types.h"

class ClickGenerator final : public bolt::Computation {
 public:
  using CtxPtr = bolt::Computation::CtxPtr;

  virtual void init(CtxPtr ctx) override {
    rand_.seed(std::random_device()());
    LOG(INFO) << "Initializing click source";
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    thrift::AdEvent event;
    event.__set_type(thrift::StreamEvent::CLICK);
    event.__set_id(randomImpression());
    std::string serEvent = toBytes(event);
    for (auto i = 0u; i < 50; ++i) {
      ctx->produceRecord("clicks", std::move(serEvent), "-");
    }
    ctx->setTimer("loop", bolt::timeNowMilli() + 1000);
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "click-generator";
    m.ostreams.insert("clicks");
    return m;
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {}

 private:
  uint64_t randomImpression() { return dist_(rand_); }

  std::mt19937 rand_;
  std::uniform_int_distribution<uint64_t> dist_;
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<ClickGenerator>(), argc,
                                 argv);
  return 0;
}
