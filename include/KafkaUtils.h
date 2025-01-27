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

inline void SendValue(KafkaProducer& producer, const kafka::Topic& topic,
                      const kafka::Key& key, const kafka::Value& value,
                      const DeliveryCallback& deliveryCb,
                      const KafkaProducer::SendOption option =
                          KafkaProducer::SendOption::ToCopyRecordValue) {
  const ProducerRecord record(topic, key, value);

  producer.send(record, deliveryCb, option);
}

template <typename T>
T ValueTo(const kafka::Value& value) {
  if (!value.data()) {
    throw std::runtime_error("Received empty value");
  }
  const std::string serializedData(static_cast<const char*>(value.data()), value.size());
  return Deserialize<T>(serializedData);
}

}  // namespace KafkaUtils
