#include <chrono>
#include <iostream>
#include <pqxx/pqxx>

using TimePoint = std::chrono::time_point<std::chrono::system_clock,
                                          std::chrono::system_clock::duration>;

class Timer {
 public:
  constexpr Timer() { m_start = std::chrono::high_resolution_clock::now(); }

  ~Timer() {
    const TimePoint end = std::chrono::high_resolution_clock::now();
    std::cout
        << "Time : "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count()
        << std::endl;
  }

 private:
  TimePoint m_start;
};

constexpr std::string FormConnectionString(std::string_view dbname, std::string_view user,
                                           std::string_view password,
                                           std::string_view host, int port) {
  return std::format("dbname={} user={} password={} host={} port={}", dbname, user,
                     password, host, port);
}

void FetchFromUsers(pqxx::work& work) {
  for (const auto [name, email] : work.stream<std::string_view, std::string_view>(
           "SELECT name, email FROM public.\"Users\"")) {
    std::cout << name << " : " << email << std::endl;
  }
}

void InsertIntoUsers(pqxx::work& work, std::string_view name, std::string_view email) {
  const auto result =
      work.exec(std::format("INSERT INTO public.\"Users\"(name, email) VALUES ({}, {});",
                            name, email))
          .no_rows();
}

int main() {
  try {
    const auto connectionString = FormConnectionString(
        "postgres_db", "postgres_user", "postgres_password", "localhost", 5430);
    pqxx::connection conn(connectionString);

    pqxx::work work{conn};

    {
      Timer t;
      FetchFromUsers(work);
    }

    {
      Timer t;
      InsertIntoUsers(work, "'Huyadze'", "'huyadze@mail.com'");
    }

    {
      Timer t;
      FetchFromUsers(work);
    }

    work.commit();

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
