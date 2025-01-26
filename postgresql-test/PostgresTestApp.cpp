#include <chrono>
#include <iostream>
#include <pqxx/pqxx>

#include "DbUtils.h"
#include "Timer.h"

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
