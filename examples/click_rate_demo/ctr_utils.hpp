#pragma once
#include <memory>
#include <string>
#include <type_traits>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>

template <typename T> inline T fromBytes(const char *value, int size) {
  using namespace ::apache::thrift::transport;
  using namespace ::apache::thrift::protocol;
  T t;
  boost::shared_ptr<TTransport> trans(
    new TMemoryBuffer((uint8_t *)value, (uint32_t)size));
  std::shared_ptr<TProtocol> proto(new TBinaryProtocol(trans));
  t.read(proto.get());
  return t;
}

template <> inline std::string fromBytes(const char *value, int size) {
  return std::string(value, size);
}

template <typename T> inline std::string toBytes(const T &t) {
  using namespace ::apache::thrift::transport;
  using namespace ::apache::thrift::protocol;
  boost::shared_ptr<TMemoryBuffer> trans(new TMemoryBuffer());
  std::shared_ptr<TProtocol> proto(new TBinaryProtocol(trans));
  t.write(proto.get());
  return trans->getBufferAsString();
}
template <> inline std::string toBytes(const std::string &t) { return t; }
