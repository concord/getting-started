#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

#include "ctr_utils.hpp"
#include "gen-cpp/click_types.h"

class CtrCalculator final : public bolt::Computation {
 public:
  using CtxPtr = bolt::Computation::CtxPtr;
  
  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing click-through-rate calculator";
    clicks_ = imps_ = 0;
    ctx->setTimer("loop", bolt::timeNowMilli());
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    LOG(INFO) << "Clicks: " << clicks_;
    LOG(INFO) << "Impressions: " << imps_;
    LOG(INFO) << "Click Through Rate: " << std::to_string(ctr()) << "%";
    ctx->setTimer("loop", bolt::timeNowMilli() + 1000);
  }
  
  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {
    const std::string payload = r.key();
    const thrift::AdEvent event =
      fromBytes<thrift::AdEvent>(payload.c_str(), payload.size());
    switch(event.type) {
    case thrift::StreamEvent::CLICK:
      clicks_++;
      break;
    case thrift::StreamEvent::IMPRESSION:
      imps_++;
      break;
    default:
      throw new std::runtime_error("Event propogated from unexpected stream");
    }
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "ctr-calculator";
    m.istreams.insert({"impressions", bolt::Grouping::ROUND_ROBIN});
    m.istreams.insert({"clicks", bolt::Grouping::ROUND_ROBIN});
    return m;
  }

 private:
  double ctr() const {
    if (imps_ == 0 || clicks_ >= imps_) {
      return 100.0;
    }
    return (static_cast<double>(clicks_) / static_cast<double>(imps_)) * 100.0;
  }
  
  uint64_t clicks_;
  uint64_t imps_;
  
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<CtrCalculator>(), argc,
                                 argv);
  return 0;
}
