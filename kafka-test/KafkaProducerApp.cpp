#include <kafka/KafkaProducer.h>

#include <iostream>
#include <string>

int main() {
  using namespace kafka;
  using namespace kafka::clients::producer;

  const std::string brokers = "localhost:29092";
  const Topic topic = "test-topic";

  // Prepare the configuration
  Properties props;
  props.put("bootstrap.servers", brokers);

  // Create a producer
  KafkaProducer producer(props);

  // Prepare delivery callback
  auto deliveryCb = [](const RecordMetadata& metadata, const Error& error) {
    if (!error) {
      std::cout << "Message delivered: " << metadata.toString() << std::endl;
    } else {
      std::cerr << "Message failed to be delivered: " << error.message() << std::endl;
    }
  };

  // Prepare a message
  std::cout << "Type message value and hit enter to produce message..." << std::endl;
  std::string line;
  std::getline(std::cin, line);

  const ProducerRecord record(topic, NullKey, Value(line.c_str(), line.size()));

  // Send a message
  producer.send(record, deliveryCb);

  // Close the producer explicitly(or not, since RAII will take care of it)
  producer.close();
}