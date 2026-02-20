#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <cstdint>

namespace io {

/// Simple file writer with line buffering and flush control.
class FileWriter {
public:
    explicit FileWriter(const std::string& path, bool append = false);
    ~FileWriter();

    FileWriter(const FileWriter&)            = delete;
    FileWriter& operator=(const FileWriter&) = delete;

    /// Write a single line (newline appended automatically).
    void writeLine(const std::string& line);

    /// Write raw bytes.
    void writeBytes(const std::vector<uint8_t>& data);

    /// Flush internal buffer to disk.
    void flush();

    bool isOpen() const noexcept;

    /// Total bytes written during this session.
    std::size_t bytesWritten() const noexcept;

private:
    std::ofstream m_file;
    std::size_t   m_bytesWritten{0};
};

} // namespace io
