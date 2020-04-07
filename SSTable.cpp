#include "SSTable.h"
#include <fstream>
#include <sstream>
#include <filesystem>

SSTable::SSTable(const SSTableId &id): id(id) {
    std::ifstream ifs(id.name(), std::ios::binary);
    ifs.read((char*) &size, sizeof(uint64_t));
    for (uint64_t i = 0; i <= size; ++i) {
        uint64_t key, offset;
        ifs.read((char*) &key, sizeof(uint64_t));
        ifs.read((char*) &offset, sizeof(uint64_t));
        keys.push_back(key);
        offsets.push_back(offset);
    }
    ifs.close();
}

SSTable::SSTable(const SkipList &memTable, const SSTableId &id): id(id) {
    size = memTable.size();
    uint64_t offset = 0;
    std::ostringstream oss;
    SkipList::Iterator itr = memTable.iterator();
    while (itr.hasNext()) {
        Entry entry = itr.next();
        keys.push_back(entry.key);
        offsets.push_back(offset);
        offset += entry.value.length();
        oss << entry.value;
    }
    keys.push_back(0);
    offsets.push_back(offset);
    save(oss.str());
}

SSTable::SSTable(const std::vector<Entry> &data, size_t &pos, uint64_t bound, const SSTableId &id): id(id) {
    size_t n = data.size();
    size = 0;
    uint64_t offset = 0;
    std::ostringstream oss;
    while (pos < n && (size * 2 + 3) * sizeof(uint64_t) + offset <= bound) {
        Entry entry = data[pos];
        keys.push_back(entry.key);
        offsets.push_back(offset);
        offset += entry.value.length();
        oss << entry.value;
        ++size;
        ++pos;
    }
    keys.push_back(0);
    offsets.push_back(offset);
    save(oss.str());
}

SearchResult SSTable::search(uint64_t key) const {
    uint64_t left = 0;
    uint64_t right = size;
    while (right - left > 2) {
        uint64_t mid = left + (right - left) / 2;
        if (keys[mid] < key)
            left = mid;
        else if (keys[mid] > key)
            right = mid;
        else
            return {true, read(mid)};
    }
    if (keys[left] == key)
        return {true, read(left)};
    else if (keys[right - 1] == key)
        return {true, read(right - 1)};
    else
        return {false, ""};
}

std::vector<Entry> SSTable::load() const {
    std::vector<Entry> ret;
    std::ifstream ifs(id.name(), std::ios::binary);
    ifs.seekg((2 * size + 3) * sizeof(uint64_t), std::ios::beg);
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t key = keys[i];
        uint64_t len = offsets[i + 1] - offsets[i];
        char *buf = new char[len + 1];
        buf[len] = '\0';
        ifs.read(buf, len);
        std::string value(buf);
        delete[] buf;
        ret.emplace_back(key, value);
    }
    ifs.close();
    return ret;
}

void SSTable::remove() const {
    std::filesystem::remove(std::filesystem::path(id.name()));
}

uint64_t SSTable::number() const {
    return id.no;
}

uint64_t SSTable::lower() const {
    return keys[0];
}

uint64_t SSTable::upper() const {
    return keys[size - 1];
}

uint64_t SSTable::space() const {
    return (size * 2 + 3) * sizeof(uint64_t) + offsets.back();
}

void SSTable::save(const std::string &values) const {
    std::ofstream ofs(id.name(), std::ios::binary);
    ofs.write((char*) &size, sizeof(uint64_t));
    for (uint64_t i = 0; i <= size; ++i) {
        ofs.write((char*) &keys[i], sizeof(uint64_t));
        ofs.write((char*) &offsets[i], sizeof(uint64_t));
    }
    ofs.write(values.c_str(), values.length());
    ofs.close();
}

std::string SSTable::read(uint64_t pos) const {
    std::ifstream ifs(id.name());
    uint64_t len = offsets[pos + 1] - offsets[pos];
    char *buf = new char[len + 1];
    buf[len] = '\0';
    ifs.seekg((2 * size + 3) * sizeof(uint64_t) + offsets[pos], std::ios::beg);
    ifs.read(buf, len);
    std::string value(buf);
    delete[] buf;
    ifs.close();
    return value;
}