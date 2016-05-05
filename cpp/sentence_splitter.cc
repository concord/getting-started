#include <memory>
#include <sstream>
#include <vector>
#include <iterator>
#include <concord/glog_init.hpp>
#include <concord/Computation.hpp>
#include <concord/time_utils.hpp>

class SentenceSplitter final : public bolt::Computation {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;
  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing sentence splitter [cpp]";
  }

  virtual void destroy() override {
    LOG(INFO) << "Destructing sentence splitter [cpp]";
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {
    std::stringstream ss(std::move(r.key));
    std::string token;
    do {
      ss >> token;
      ctx->produceRecord("words", token, "");
    } while(ss);
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {}

  virtual bolt::Metadata metadata() override {
    bolt::Metadata m;
    m.name = "word-splitter";
    m.istreams.insert({"sentences", bolt::Grouping::SHUFFLE});
    m.ostreams.insert("words");
    return m;
  }
};

int main(int argc, char *argv[]) {
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<SentenceSplitter>(), argc,
                                 argv);
  return 0;
}
