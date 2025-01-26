#pragma once

#include <format>
#include <pqxx/pqxx>
#include <string>

inline std::string FormConnectionString(std::string_view dbname, std::string_view user,
                                        std::string_view password, std::string_view host,
                                        int port) {
  return std::format("dbname={} user={} password={} host={} port={}", dbname, user,
                     password, host, port);
}

inline std::string GetDbVersion(pqxx::work& work) {
  const pqxx::result res = work.exec("SELECT version()");
  return res.front().front().c_str();
}