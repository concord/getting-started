#include <unordered_map>
#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

#include "ctr_utils.hpp"
#include "gen-cpp/click_types.h"

/**
 * Small struct for grouping data and functions relating to the calculation
 * of a click through rate
 */
struct ProviderEvents {
  public:
  ProviderEvents() : clicks_(0), impressions_(0) {}

  void incClicks() { ++clicks_; }
  void incImpressions() { ++impressions_; }

  uint32_t getClicks() const { return clicks_; }
  uint32_t getImpressions() const { return impressions_; }
  
  double ctr() const {
    if(impressions_ == 0 || clicks_ >= impressions_) {
      return 100.0L;
    }
    return (static_cast<double>(clicks_) / static_cast<double>(impressions_))
           * 100.0;
  }

  private:
  uint32_t clicks_;
  uint32_t impressions_;
};

/**
 * Concord computation that will calculate the click through rate for a 
 * given set of providers.
 */
class CtrCalculator final : public bolt::Computation {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;

  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing click-through-rate calculator";
    ctx->setTimer("loop", bolt::timeNowMilli() + 1000);
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {
    // Print click through rate metrics per provider
    for(const auto &provider : providerCtr_) { 
      const auto clicks = provider.second->getClicks();
      const auto impressions = provider.second->getImpressions();
      if (clicks < impressions) {
	LOG(INFO) << "Data for provider [" << provider.first << "]";
	LOG(INFO) << "Clicks: " << clicks;
	LOG(INFO) << "Impressions: " << impressions;
	LOG(INFO) << "Click Through Rate: "
		  << std::to_string(provider.second->ctr()) << "%";
      }
    }
    providerCtr_.clear();
    ctx->setTimer("loop", bolt::timeNowMilli() + 1000);
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {
    // Deserialize thrfit struct
    const std::string &provider = r.key();
    const std::string &payload = r.value();

    const thrift::AdEvent event =
      fromBytes<thrift::AdEvent>(payload.c_str(), payload.size());

    // Look up by provider, creating new struct if current provider dne
    auto &pEvent = providerCtr_[provider];
    if(!pEvent) {
      pEvent = std::make_unique<ProviderEvents>();
    }

    // Increment counter for detected event by current provider
    switch(event.type) {
    case thrift::StreamEvent::CLICK:
      pEvent->incClicks();
      break;
    case thrift::StreamEvent::IMPRESSION:
      pEvent->incImpressions();
      break;
    default:
      throw new std::runtime_error("Unhandled event intercepted");
    }
  }

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "ctr-calculator";
    m.istreams.insert({"impressions", bolt::Grouping::GROUP_BY});
    m.istreams.insert({"clicks", bolt::Grouping::GROUP_BY});
    return m;
  }

  private:
  // Aggregating ad events by provider name
  std::unordered_map<std::string, std::unique_ptr<ProviderEvents>> providerCtr_;
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<CtrCalculator>(), argc, argv);
  return 0;
}
