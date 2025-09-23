#pragma once
#include <QObject>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QDesktopServices>
#include <QUrl>

#include <Application/QTUtils/ProxyModel.h>

class ContentBrowserViewModel : public QObject 
{
    Q_OBJECT
        Q_PROPERTY(QString currentDir READ currentDir WRITE setCurrentDir NOTIFY currentDirChanged)
        Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
public:
    explicit ContentBrowserViewModel(QObject* parent = nullptr) : QObject(parent) {}

    void setModels(QFileSystemModel* fs, FileFilterProxyModel* proxy) 
    {
        m_fs = fs; m_proxy = proxy;
        connect(m_proxy, &FileFilterProxyModel::filterTextChanged, this, &ContentBrowserViewModel::updateStatus);
        connect(m_fs, &QFileSystemModel::directoryLoaded, this, &ContentBrowserViewModel::updateStatus);
        updateStatus();
    }

    QString currentDir()  const { return m_currentDir; }
    QString statusText()  const { return m_statusText; }

public slots:
    void setCurrentDir(const QString& dirPath) 
    {
        if (!m_fs) return;
        QString normalized = QDir::fromNativeSeparators(dirPath);
        if (normalized.isEmpty() || normalized == m_currentDir) return;
        if (!QDir(normalized).exists()) normalized = QDir::homePath();
        m_currentDir = normalized;
        emit currentDirChanged(m_currentDir);
        updateStatus();
    }

    void setFilterText(const QString& text) 
    {
        if (m_proxy) m_proxy->setFilterText(text);
        updateStatus();
    }

    void openPath(const QString& path) 
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    }

signals:
    void currentDirChanged(const QString&);
    void statusTextChanged(const QString&);

private:
    void updateStatus() 
    {
        if (!m_fs) 
            return;

        const QFileInfo fi(m_currentDir);
        const QString base = fi.exists() ? fi.absoluteFilePath() : QString("<invalid>");
        const QString filter = (m_proxy && !m_proxy->filterText().isEmpty()) ? m_proxy->filterText() : "<none>";

        m_statusText = QString("Dir: %1  |  Filter: %2").arg(base, filter);
        emit statusTextChanged(m_statusText);
    }

    QFileSystemModel* m_fs{ nullptr };
    FileFilterProxyModel* m_proxy{ nullptr };
    QString               m_currentDir;
    QString               m_statusText;
};