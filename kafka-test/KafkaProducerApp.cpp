#include <DbUtils.h>
#include <kafka/KafkaProducer.h>

#include <iostream>
#include <pqxx/pqxx>
#include <string>

#include "KafkaUtils.h"

using namespace kafka::clients::producer;

std::vector<std::string> CreateValues(pqxx::work& work) {
  std::vector<std::string> values;

  for (const auto [name, email] : work.stream<std::string_view, std::string_view>(
           "SELECT name, email FROM public.\"Users\"")) {
    values.push_back(std::format("{} - {}", name, email));
  }

  return values;
}

void SendValues(pqxx::work& work, const kafka::Topic& topic, KafkaProducer& producer) {
  // Prepare delivery callback
  auto deliveryCb = [](const RecordMetadata& metadata, const kafka::Error& error) {
    if (!error) {
      std::cout << "Message delivered: " << metadata.toString() << std::endl;
    } else {
      std::cerr << "Message failed to be delivered: " << error.message() << std::endl;
    }
  };

  {
    std::cout << "Sending messages with FLOAT type" << std::endl;

    constexpr float f = 2.34;

    kafka::extensions::SendValue(producer, topic, kafka::NullKey,
                                 kafka::extensions::ValueFrom(f).AsConstBuffer(),
                                 deliveryCb);
  }

  {
    std::cout << "Sending messages with STRINGS type" << std::endl;

    for (const auto values = CreateValues(work); const auto& value : values) {
      kafka::extensions::SendValue(producer, topic, kafka::NullKey,
                                   kafka::extensions::ValueFrom(value).AsConstBuffer(),
                                   deliveryCb);
    }
  }
}

void DoProducerWork(pqxx::work& work) {
  const std::string brokers = "localhost:29092";
  const kafka::Topic topic = "test-topic";

  // Prepare the configuration
  kafka::Properties props;
  props.put("bootstrap.servers", brokers);

  // Create a producer
  KafkaProducer producer(props);

  std::println(std::cout, "SendValues function");
  SendValues(work, topic, producer);
}

int main() {
  try {
    const auto connectionString = FormConnectionString(
        "postgres_db", "postgres_user", "postgres_password", "localhost", 5430);
    pqxx::connection conn(connectionString);

    pqxx::work work{conn};

    DoProducerWork(work);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
