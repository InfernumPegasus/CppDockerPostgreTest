#pragma once

#include <string>
#include <sstream>

// Шаблонные функции для сериализации и десериализации
template <typename T>
std::string Serialize(const T& value) {
  static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
  std::ostringstream oss;
  oss.write(reinterpret_cast<const char*>(&value), sizeof(T));
  return oss.str();
}

template <typename T>
T Deserialize(const std::string& serializedData) {
  static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
  T value;
  std::istringstream iss(serializedData);
  iss.read(reinterpret_cast<char*>(&value), sizeof(T));
  return value;
}

// Специализация для строк (std::string)
template <>
inline std::string Serialize<std::string>(const std::string& value) {
  return value; // Для строк сериализация — это просто копирование
}

template <>
inline std::string Deserialize<std::string>(const std::string& serializedData) {
  return serializedData; // Для строк десериализация — это просто копирование
}