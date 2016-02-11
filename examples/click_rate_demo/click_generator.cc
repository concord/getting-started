#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

#include "ctr_utils.hpp"
#include "generator.hpp"

class ClickGenerator final : public bolt::Computation, private Generator {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;

  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing click source";
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    const auto event = newEvent(thrift::StreamEvent::CLICK, randomImpression());
    auto serializedEvent = toBytes(event);
    for(auto i = 0u; i < numberOfClicks(); ++i) {
      ctx->produceRecord("clicks", randomPublisher(),
                         std::move(serializedEvent));
    }
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "click-generator";
    m.ostreams.insert("clicks");
    return m;
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {}
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<ClickGenerator>(), argc,
                                 argv);
  return 0;
}
