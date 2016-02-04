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
    LOG(INFO) << "Click Through Rate: " << std::to_string(ctr());
    ctx->setTimer("loop", bolt::timeNowMilli() * 1000);
  }
  
  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {
    const char *recordData = r.value().c_str();
    const thrift::AdEvent event = fromBytes<thrift::AdEvent>(
        recordData, sizeof(char) * strlen(recordData));
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
    return m;
  }

 private:
  double ctr() const { return (clicks_/imps_) * (double)100.0; }
  
  uint64_t clicks_;
  uint64_t imps_;
  
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<CtrCalculator>(), argc,
                                 argv);
  return 0;
}
