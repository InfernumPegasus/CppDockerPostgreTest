#include <KafkaUtils.h>
#include <kafka/KafkaConsumer.h>
#include <signal.h>

#include <iostream>
#include <string>

std::atomic_bool running = {true};

void StopRunning(int sig) {
  if (sig != SIGINT) return;

  if (running) {
    running = false;
  } else {
    // Restore the signal handler, -- to avoid stuck with this handler
    signal(SIGINT, SIG_IGN);  // NOLINT
  }
}

void DoConsumerWork() {
  using namespace kafka;
  using namespace kafka::clients::consumer;

  const std::string brokers = "localhost:29092";
  const Topic topic = "test-topic";

  // Prepare the configuration
  Properties props;
  props.put("bootstrap.servers", brokers);

  // Create a consumer instance
  KafkaConsumer consumer(props);

  // Subscribe to topics
  consumer.subscribe({topic});

  while (running) {
    // Poll messages from Kafka brokers
    for (auto records = consumer.poll(std::chrono::milliseconds(100));
         const auto& record : records) {
      if (!record.error()) {
        std::cout << "Got a new message..." << std::endl;
        std::cout << "    Topic    : " << record.topic() << std::endl;
        std::cout << "    Partition: " << record.partition() << std::endl;
        std::cout << "    Offset   : " << record.offset() << std::endl;
        std::cout << "    Timestamp: " << record.timestamp().toString() << std::endl;
        std::cout << "    Headers  : " << toString(record.headers()) << std::endl;

        try {
          std::string stringValue =
              KAFKA_API::extensions::ValueTo<std::string>(record.value());
          std::cout << "    STRING [" << stringValue << "]" << std::endl;
        } catch (const std::exception& e) {
          std::cerr << "    Failed to deserialize as string: " << e.what() << std::endl;
        }

        try {
          float floatValue = KAFKA_API::extensions::ValueTo<float>(record.value());
          std::cout << "    FLOAT [" << floatValue << "]" << std::endl;
        } catch (const std::exception& e) {
          std::cerr << "    Failed to deserialize as float: " << e.what() << std::endl;
        }

      } else {
        std::cerr << record.toString() << std::endl;
      }
    }
  }
}

int main() {
  // Use Ctrl-C to terminate the program
  signal(SIGINT, StopRunning);  // NOLINT

  DoConsumerWork();

  return 0;
}
