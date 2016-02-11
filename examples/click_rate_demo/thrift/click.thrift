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

const map<i32, Publisher> INT_TO_PUBLISHER = { 0 : Publisher.ADSTERRA, 1 : Publisher.UNDERTONE, 2 : Publisher.CLICKSOR, 3 : Publisher.NEXUS, 4 : Publisher.CONVERSANT };

struct AdEvent {
  1: StreamEvent type;
  2: i64 id;
}