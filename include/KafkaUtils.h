#pragma once

#include <kafka/KafkaProducer.h>

#include "SerializationUtils.h"

namespace KafkaUtils {

using namespace kafka::clients::producer;

using DeliveryCallback = std::function<void(const RecordMetadata&, const kafka::Error&)>;

template <typename T>
void SendValue(KafkaProducer& producer, const kafka::Topic& topic, const kafka::Key& key,
               const T& value, const DeliveryCallback& deliveryCb,
               const KafkaProducer::SendOption option =
                   KafkaProducer::SendOption::ToCopyRecordValue) {
  const auto serializedValue = Serialize(value);

  const ProducerRecord record(
      topic, key, kafka::Value(serializedValue.data(), serializedValue.size()));

  producer.send(record, deliveryCb, option);
}

template <typename T>
T ValueTo(const kafka::Value& value) {
  if (!value.data()) {
    throw std::runtime_error("Received empty value");
  }

  const std::vector<std::byte> serializedData(
      static_cast<const std::byte*>(value.data()),
      static_cast<const std::byte*>(value.data()) + value.size());

  return Deserialize<T>(serializedData);
}

}  // namespace KafkaUtils
