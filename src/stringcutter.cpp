#include <codecvt>
#include <locale>
#include <string>

static bool isUTF8Continuation(unsigned char c);
static size_t UTF8CharSize(unsigned char c);
static std::string truncateUTF8Bytes(const std::string& s, size_t max_bytes);

static bool isUTF8Continuation(unsigned char c) {
    return (c & 0xC0) == 0x80;
}

static size_t UTF8CharSize(unsigned char c) {
    if ((c & 0x80) == 0x00) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

static std::string truncateUTF8Bytes(const std::string& s, size_t max_bytes) {
    size_t i = 0;
    size_t used = 0;

    while (i < s.size()) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        size_t len = UTF8CharSize(c);

        if (used + len > max_bytes) break;

        i += len;
        used += len;
    }

    return s.substr(0, i);
}

static std::string truncateUTF16Bytes(const std::string& input, size_t max_bytes) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::wstring w = conv.from_bytes(input);
    size_t maxChars = max_bytes / 2;

    if (w.size() > maxChars) {
        w = w.substr(0, maxChars);
    }

    return conv.to_bytes(w);
}