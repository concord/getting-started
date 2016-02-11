// This file defines a common interface between computations in
// the click_through_rate demo

namespace cpp thrift

enum StreamEvent {
  CLICK = 0,
  IMPRESSION
}

struct AdEvent {
  1: required StreamEvent type;
  2: required i64 id;
}