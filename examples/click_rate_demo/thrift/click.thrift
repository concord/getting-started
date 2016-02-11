// This file defines a common interface between computations in
// the click_through_rate demo

namespace cpp thrift

enum StreamEvent {
  CLICK = 0,
  IMPRESSION
}

enum Publisher {
  ADSTERRA = 0,
  UNDERTONE,
  CLICKSOR,
  NEXUS,
  CONVERSANT
}

const map<i32, string> PUBLISHER_TO_STRING = { 0 : "ADSTERRA", 1 : "UNDERTONE", 2 : "CLICKSOR", 3 : "NEXUS", 4 : "CONVERSANT" };

const map<string, i32> STRING_TO_PUBLISHER = { "ADSTERRA" : 0, "UNDERTONE" : 1, "CLICKSOR" : 2, "NEXUS" : 3, "CONVERSANT" : 4 };

struct AdEvent {
  1: StreamEvent type;
  2: i64 id;
}