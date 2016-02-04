// This file defines a common interface between computations in
// the click_through_rate demo

namespace cpp thrift

enum StreamEvent {
  CLICK,
  IMPRESSION
}

struct AdEvent {
  1: StreamEvent type;
  2: i64 id;
}