#ifndef DAP_FS_H
#define DAP_FS_H

#include <QString>
#include <QDir>

#ifndef USE_QT_FILESYSTEM
// If std::filesystem is available, just alias it
#include <filesystem>
namespace dap_fs = std::filesystem;
#else
// If no std::filesystem, provide our Qt-based implementation
namespace dap_fs {

using path_t = QString;  // Internal path type

class path {
public:
    path() = default;
    path(const QString& p);
    path(const char* p);
    path(const path& other);
    path(const std::string& p);
    
    path& operator=(const path& other) noexcept = default;
    
    path operator/(const path& other) const;
    path& operator/=(const path& other);
    
    path extension() const;
    path filename() const;
    path stem() const;
    path parent_path() const;
    
    std::string string() const;
    std::string generic_string() const;
    QString qstring() const;
    
    operator std::string() const { return string(); }

private:
    path_t m_path;
    friend class directory_entry;
};

enum class file_type {
    none, not_found, regular, directory, symlink,
    block, character, fifo, socket, unknown
};

enum class perms {
    none = 0,
    owner_read = 0400, owner_write = 0200, owner_exec = 0100,
    group_read = 040, group_write = 020, group_exec = 010,
    others_read = 04, others_write = 02, others_exec = 01,
    all = 0777, mask = 0777, unknown = 0xFFFF
};

class file_status {
public:
    file_status() noexcept = default;
    explicit file_status(file_type ft, perms prms = perms::unknown) noexcept
        : m_type(ft), m_perms(prms) {}
    
    file_type type() const noexcept { return m_type; }
    perms permissions() const noexcept { return m_perms; }

private:
    file_type m_type = file_type::none;
    perms m_perms = perms::unknown;
};

class directory_entry {
public:
    directory_entry() = default;
    explicit directory_entry(const class path& p) : m_path(p) {}
    
    const class path& get_path() const noexcept { return m_path; }
    const class path& path() const noexcept { return m_path; }
    operator const class path&() const noexcept { return m_path; }
    
    bool exists() const { return QFileInfo::exists(m_path.qstring()); }
    bool is_regular_file() const { return QFileInfo(m_path.qstring()).isFile(); }
    bool is_directory() const { return QFileInfo(m_path.qstring()).isDir(); }
    
    file_status status() const;

private:
    class path m_path;
};

class directory_iterator {
public:
    directory_iterator() = default;
    explicit directory_iterator(const path& p);
    
    directory_iterator& operator++();
    bool operator!=(const directory_iterator& other) const;
    const directory_entry& operator*() const;
    
    static directory_iterator end() { return directory_iterator(); }

private:
    QDir m_dir;
    QStringList m_entries;
    int m_current = 0;
    directory_entry m_current_entry;
};

// Free-standing functions
inline directory_iterator begin(directory_iterator iter) { return iter; }
inline directory_iterator end(directory_iterator) { return directory_iterator(); }

bool exists(const path& p);
bool exists(file_status s) noexcept;
bool is_directory(const path& p);
bool is_directory(file_status s) noexcept;
bool is_regular_file(const path& p);
bool is_regular_file(file_status s) noexcept;
path weakly_canonical(const path& p);

} // namespace dap_fs
#endif // USE_QT_FILESYSTEM
#endif // DAP_FS_H