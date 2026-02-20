#include "io/file_writer.h"
#include <stdexcept>

namespace io {

FileWriter::FileWriter(const std::string& path, bool append) {
    const auto mode = append
        ? (std::ios::out | std::ios::app)
        : (std::ios::out | std::ios::trunc);
    m_file.open(path, mode);
    if (!m_file.is_open()) {
        throw std::runtime_error("FileWriter: cannot open file: " + path);
    }
}

FileWriter::~FileWriter() {
    if (m_file.is_open()) {
        m_file.flush();
        m_file.close();
    }
}

void FileWriter::writeLine(const std::string& line) {
    const std::string out = line + "\n";
    m_file.write(out.data(), static_cast<std::streamsize>(out.size()));
    m_bytesWritten += out.size();
}

void FileWriter::writeBytes(const std::vector<uint8_t>& data) {
    m_file.write(
        reinterpret_cast<const char*>(data.data()),
        static_cast<std::streamsize>(data.size())
    );
    m_bytesWritten += data.size();
}

void FileWriter::flush() {
    m_file.flush();
}

bool FileWriter::isOpen() const noexcept {
    return m_file.is_open();
}

std::size_t FileWriter::bytesWritten() const noexcept {
    return m_bytesWritten;
}

} // namespace io
