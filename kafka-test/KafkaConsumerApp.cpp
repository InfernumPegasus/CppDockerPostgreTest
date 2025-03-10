#include <kafka/KafkaConsumer.h>
#include <signal.h>

#include <iostream>
#include <string>

std::atomic_bool running = {true};

void stopRunning(int sig) {
  if (sig != SIGINT) return;

  if (running) {
    running = false;
  } else {
    // Restore the signal handler, -- to avoid stuck with this handler
    signal(SIGINT, SIG_IGN);  // NOLINT
  }
}

int main() {
  using namespace kafka;
  using namespace kafka::clients::consumer;

  // Use Ctrl-C to terminate the program
  signal(SIGINT, stopRunning);  // NOLINT

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
        std::cout << "    Key   [" << record.key().toString() << "]" << std::endl;
        std::cout << "    Value [" << record.value().toString() << "]" << std::endl;
      } else {
        std::cerr << record.toString() << std::endl;
      }
    }
  }

  // No explicit close is needed, RAII will take care of it
  consumer.close();
}
