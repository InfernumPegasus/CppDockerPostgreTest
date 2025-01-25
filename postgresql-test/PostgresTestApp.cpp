#include <chrono>
#include <iostream>
#include <pqxx/pqxx>

class Timer {
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

 public:
  Timer() { m_start = Clock::now(); }

  ~Timer() {
    const TimePoint end = Clock::now();
    std::cout
        << "Time : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count()
        << std::endl;
  }

 private:
  TimePoint m_start;
};

std::string FormConnectionString(std::string_view dbname, std::string_view user,
                                 std::string_view password, std::string_view host,
                                 int port) {
  return std::format("dbname={} user={} password={} host={} port={}", dbname, user,
                     password, host, port);
}

std::string GetDbVersion(pqxx::work& work) {
  const pqxx::result res = work.exec("SELECT version()");
  return res.front().front().c_str();
}

int main() {
  try {
    const auto connectionString = FormConnectionString(
        "postgres_db", "postgres_user", "postgres_password", "localhost", 5430);
    pqxx::connection conn(connectionString);

    pqxx::work work{conn};

    {
      Timer t;
      std::cout << "DB version: " << GetDbVersion(work) << std::endl;
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
