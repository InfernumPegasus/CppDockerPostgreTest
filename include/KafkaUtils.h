#pragma once

#include <kafka/KafkaProducer.h>

#include "OwningBuffer.h"
#include "SerializationUtils.h"

namespace KAFKA_API {
namespace extensions {

using namespace kafka::clients::producer;

using DeliveryCallback = std::function<void(const RecordMetadata&, const Error&)>;

inline void SendValue(
    KafkaProducer& producer, const Topic& topic, const Key& key, const Value& value,
    const DeliveryCallback& deliveryCb,
    const KafkaProducer::SendOption option = KafkaProducer::SendOption::ToCopyRecordValue,
    const KafkaProducer::ActionWhileQueueIsFull action =
        KafkaProducer::ActionWhileQueueIsFull::Block) {
  const ProducerRecord record(topic, key, value);

  producer.send(record, deliveryCb, option, action);
}

template <typename T>
T ValueTo(const Value& value) {
  if (!value.data()) {
    throw std::runtime_error("Received empty value");
  }

  const std::vector<std::byte> serializedData(
      static_cast<const std::byte*>(value.data()),
      static_cast<const std::byte*>(value.data()) + value.size());

  return Deserialize<T>(serializedData);
}

template <typename T>
OwningBuffer ValueFrom(const T& value) {
  const auto serialized = Serialize(value);

  return OwningBuffer(serialized);
}

}  // namespace extensions
}  // namespace KAFKA_API