#include "DapFs.h"
#include <QFileInfo>

#ifdef USE_QT_FILESYSTEM
namespace dap_fs {


// ================ Qt native implementation ================

path::path(const QString& p) : m_path(QDir::cleanPath(p)) {}
path::path(const char* p) : m_path(QDir::cleanPath(QString::fromUtf8(p))) {}
path::path(const path& other) : m_path(other.m_path) {}
path::path(const std::string& p) : m_path(QDir::cleanPath(QString::fromStdString(p))) {}

path path::operator/(const path& other) const {
    return path(QDir::cleanPath(m_path + QDir::separator() + other.m_path));
}

path& path::operator/=(const path& other) {
    m_path = QDir::cleanPath(m_path + QDir::separator() + other.m_path);
    return *this;
}

path path::extension() const {
    QFileInfo fi(m_path);
    QString ext = fi.suffix();
    return ext.isEmpty() ? path() : path("." + ext);
}

path path::filename() const {
    return path(QFileInfo(m_path).fileName());
}

path path::stem() const {
    return path(QFileInfo(m_path).completeBaseName());
}

path path::parent_path() const {
    return path(QFileInfo(m_path).path());
}

QString path::qstring() const {
    return m_path;
}

std::string path::string() const {
    return m_path.toStdString();
}

std::string path::generic_string() const {
    return m_path.toStdString();
}

bool exists(const path& p) {
    return QFileInfo::exists(p.qstring());
}

bool is_directory(const path& p) {
    return QFileInfo(p.qstring()).isDir();
}

bool is_regular_file(const path& p) {
    return QFileInfo(p.qstring()).isFile();
}

path weakly_canonical(const path& p) {
    return path(QFileInfo(p.qstring()).canonicalFilePath());
}

file_status directory_entry::status() const {
    QFileInfo fi(m_path.qstring());
    if (!fi.exists())
        return file_status(file_type::not_found);
    if (fi.isFile())
        return file_status(file_type::regular);
    if (fi.isDir())
        return file_status(file_type::directory);
    return file_status(file_type::unknown);
}

directory_iterator::directory_iterator(const path& p)
    : m_dir(p.qstring())
    , m_entries(m_dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot))
    , m_current(0)
{
    if (m_current < m_entries.size()) {
        m_current_entry = directory_entry(path(m_dir.filePath(m_entries[m_current])));
    }
}

directory_iterator& directory_iterator::operator++() {
    ++m_current;
    if (m_current < m_entries.size()) {
        m_current_entry = directory_entry(path(m_dir.filePath(m_entries[m_current])));
    }
    return *this;
}

bool directory_iterator::operator!=(const directory_iterator& /*other*/) const {
    if (m_entries.isEmpty()) return false;
    return m_current < m_entries.size();
}

const directory_entry& directory_iterator::operator*() const {
    return m_current_entry;
}

bool exists(file_status s) noexcept {
    return s.type() != file_type::not_found && s.type() != file_type::none;
}

bool is_directory(file_status s) noexcept {
    return s.type() == file_type::directory;
}

bool is_regular_file(file_status s) noexcept {
    return s.type() == file_type::regular;
}


} // namespace dap_fs


#else
// ================ std::filesystem implementation ================
// same as std::filesystem
#endif // USE_QT_FILESYSTEM
