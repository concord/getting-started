// Hai!

namespace cpp thrift

// const string kBoltEnvKeyBasePath            = "BOLT_BASE_PATH";

// exception BoltError {
//   1: string reason;
//   2: string context; // typically line/ code related for debug
//   3: i64 time;
// }

// enum BackPressure {
//   NONE,
//   ENQUEUE,
//   DROP_HEAD,
//   DROP_TAIL,
//   BLOCK_SENDER,
// }

// struct Endpoint {
//   1: string ip;
//   2: i32 port;
  // }

enum StreamEvent {
  CLICK,
  IMPRESSION
}

struct AdEvent {
  1: StreamEvent type;
  2: i64 id;
}