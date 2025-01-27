#pragma once

#include <cstring>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename T>
std::vector<std::byte> Serialize(const T& value) {
  static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");

  auto begin = reinterpret_cast<const std::byte*>(&value);
  return {begin, begin + sizeof(T)};
}

template <typename T>
T Deserialize(const std::vector<std::byte>& serializedData) {
  static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable");
  static_assert(std::is_default_constructible_v<T>, "Type must be default constructible");

  if (serializedData.size() != sizeof(T)) {
    throw std::runtime_error("Serialized data size does not match target type size");
  }

  T value;
  std::memcpy(&value, serializedData.data(), sizeof(T));
  return value;
}

template <>
inline std::vector<std::byte> Serialize<std::string>(const std::string& value) {
  auto begin = reinterpret_cast<const std::byte*>(value.data());
  return {begin, begin + value.size()};
}

template <>
inline std::string Deserialize<std::string>(
    const std::vector<std::byte>& serializedData) {
  return std::string(reinterpret_cast<const char*>(serializedData.data()),
                     serializedData.size());
}