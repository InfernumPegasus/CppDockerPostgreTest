#pragma once

#include <kafka/Types.h>

#include <cstddef>
#include <string>
#include <vector>

namespace KAFKA_API {
namespace extensions {

class OwningBuffer {
 public:
  explicit OwningBuffer(const void* data = nullptr, const std::size_t size = 0) {
    if (data && size > 0) {
      m_rawData.assign(static_cast<const std::byte*>(data),
                       static_cast<const std::byte*>(data) + size);
    }
  }

  explicit OwningBuffer(const ConstBuffer& buffer)
      : OwningBuffer(buffer.data(), buffer.size()) {}

  explicit OwningBuffer(const std::vector<std::byte>& bytes) : m_rawData(bytes) {}

  explicit OwningBuffer(std::vector<std::byte>&& bytes) noexcept
      : m_rawData(std::move(bytes)) {}

  const void* Data() const { return m_rawData.data(); }
  std::size_t Size() const { return m_rawData.size(); }

  std::string ToString() const {
    const ConstBuffer buffer(m_rawData.data(), m_rawData.size());
    return buffer.toString();
  }

  ConstBuffer AsConstBuffer() const {
    return ConstBuffer(m_rawData.data(), m_rawData.size());
  }

 private:
  std::vector<std::byte> m_rawData;
};

}  // namespace extensions
}  // namespace KAFKA_API
