#include <DbUtils.h>
#include <kafka/KafkaProducer.h>

#include <iostream>
#include <pqxx/pqxx>
#include <string>

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

    // Raw pointers usage test
    const void* fPtr = &f;

    // Smart pointers usage test
    auto fUptr = std::make_unique<float>(f);

    const ProducerRecord r1(topic, kafka::NullKey, kafka::Value(fPtr, sizeof(f)));
    const ProducerRecord r2(topic, kafka::NullKey, kafka::Value(fUptr.get(), sizeof(f)));

    producer.send(r1, deliveryCb, KafkaProducer::SendOption::ToCopyRecordValue);
    producer.send(r2, deliveryCb, KafkaProducer::SendOption::ToCopyRecordValue);
  }

  {
    std::cout << "Sending messages with STRINGS type" << std::endl;

    for (const auto values = CreateValues(work); const auto& value : values) {
      const ProducerRecord record(topic, kafka::NullKey,
                                  kafka::Value(value.c_str(), value.size()));

      // Send a message
      producer.send(record, deliveryCb, KafkaProducer::SendOption::ToCopyRecordValue);
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
