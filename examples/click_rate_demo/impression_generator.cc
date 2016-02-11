#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

#include "ctr_utils.hpp"
#include "generator.hpp"

class ImpressionGenerator final : public bolt::Computation, private Generator {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;

  ImpressionGenerator(const std::string &publisher_enum) {
    const auto publisher = thrift::g_click_constants.INT_TO_PUBLISHER.find(
      std::stoi(publisher_enum));
    if(publisher == thrift::g_click_constants.INT_TO_PUBLISHER.end()) {
      throw new std::runtime_error("Publisher does not exist for given value");
    }
    publisher_ =
      thrift::g_click_constants.PUBLISHER_TO_STRING.at(publisher->second);
  }

  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing impression source";
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    const auto event = newEvent(thrift::StreamEvent::IMPRESSION, randomImpression());
    auto serializedEvent = toBytes(event);
    for(auto i = 0u; i < 5000; ++i) {
      ctx->produceRecord("impressions", publisher_, std::move(serializedEvent));
    }
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "impression-generator";
    m.ostreams.insert("impressions");
    return m;
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {}

  private:
  std::string publisher_;
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  if(argc < 2) {
    LOG(ERROR) << "Must supply publisher enum";
    return 1;
  }
  bolt::client::serveComputation(std::make_shared<ImpressionGenerator>(argv[1]),
                                 argc, argv);
  return 0;
}
