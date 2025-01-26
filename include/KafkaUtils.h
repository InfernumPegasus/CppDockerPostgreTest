#pragma once

#include <kafka/KafkaProducer.h>

#include "SerializationUtils.h"

// Высокоуровневая обертка для отправки данных
template <typename T>
void SendValue(kafka::clients::producer::KafkaProducer& producer,
               const kafka::Topic& topic, const T& value,
               const std::function<void(const kafka::clients::producer::RecordMetadata&,
                                        const kafka::Error&)>& deliveryCb) {
  const auto serializedValue = Serialize(value);
  const kafka::clients::producer::ProducerRecord record(
      topic, kafka::NullKey,
      kafka::Value(serializedValue.data(), serializedValue.size()));
  producer.send(record, deliveryCb,
                kafka::clients::producer::KafkaProducer::SendOption::ToCopyRecordValue);
}

// Высокоуровневая обертка для получения данных из Kafka
template <typename T>
T ReceiveValue(const kafka::Value& value) {
  if (!value.data()) {
    throw std::runtime_error("Received empty value");
  }
  const std::string serializedData(static_cast<const char*>(value.data()), value.size());
  return Deserialize<T>(serializedData);
}