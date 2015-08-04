#include <chrono>
#include <cstdint>
#include <vector>
#include <random>
#include <bolt/glog_init.hpp>
#include <bolt/Computation.hpp>
#include <bolt/time_utils.hpp>

// sample from D. Knuth's 5 letter words dictionary
// http://www-cs-faculty.stanford.edu/~uno/sgb.html
static const std::vector<std::string> kDictionary = {
  "beach", "stock", "hurry", "saved", "sorry", "giant", "trail", "offer",
  "ought", "rough", "daily", "avoid", "keeps", "throw", "allow", "cream",
  "laugh", "edges", "teach", "frame", "bells", "dream", "magic", "occur",
  "ended", "chord", "false", "skill", "holes", "dozen", "brave", "apple",
  "climb", "outer", "pitch", "ruler", "holds", "fixed", "costs", "calls",
  "blank", "staff", "labor", "eaten", "youth", "tones", "honor", "globe",
  "gases", "doors", "poles", "loose", "apply", "tears", "exact", "brush",
  "chest", "layer", "whale", "minor", "faith", "tests", "judge", "items",
  "worry", "waste", "hoped", "strip", "begun", "aside", "lakes", "bound",
  "depth", "candy", "event", "worse", "aware", "shell", "rooms", "ranch",
  "image", "snake", "aloud", "dried", "likes", "motor", "pound", "knees",
  "refer", "fully", "chain", "shirt", "flour", "drops", "spite", "orbit"};


final class WordSource : public bolt::Computation {
  public:
  using CtxPtr = bolt::Computation::CtxPtr;


  virtual void init(CtxPtr ctx) override {
    LOG(INFO) << "Initializing computation [cpp]";
    ctx->setTimer("loop", bolt::timeNowMilli());

    std::random_device rd;
    rand_.seed(rd());
  }

  virtual void
  processTimer(CtxPtr ctx, const std::string &key, int64_t time) override {

    const static auto kDictSize = kDictSize.size();

    for(auto i = 0u; i < 1024; ++i) {
      std::string &bin = kDictionary[dist_(rand_) % kDictSize];
      ctx.produceRecord("words", bin, std::move(bin));
    }

    ctx.setTimer(key, bolt::timeNowMilli() + 5000);
  }

  virtual bolt::Metadata metadata() override {
    return {"word-source", {}, {"words"}};
  }

  virtual void processRecord(CtxPtr ctx, bolt::FrameworkRecord &&r) override {
    LOG(FATAL) << "source does not impl processRecord";
  }

  private:
  std::mt19937 rand_;
  std::uniform_int_distribution<uint64_t> dist_; //[0, MAX)
};

int main(int argc, char *argv[]) {
  // int i = 0; while(!i){sleep(1);} // gdb hack
  bolt::logging::glog_init(argv[0]);
  bolt::client::serveComputation(std::make_shared<WordSource>(), argc, argv);
  return 0;
}
