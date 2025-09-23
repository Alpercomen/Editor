#pragma once
#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QTableView>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QToolBar>
#include <QAction>
#include <QStyle>
#include <QStatusBar>
#include <QDir>
#include <QFileInfo>
#include <QLocale>

#include <Application/QTUtils/ProxyModel.h>
#include <Application/QTUtils/ViewModel.h>

class MainWindow : public QMainWindow 
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        setWindowTitle("Content Browser");
        resize(1200, 700);

        // Models
        fsModel = new QFileSystemModel(this);
        fsModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs);
        fsModel->setRootPath(QDir::homePath());

        proxy = new FileFilterProxyModel(this);
        proxy->setSourceModel(fsModel);
        proxy->setDynamicSortFilter(true);
        proxy->sort(0, Qt::AscendingOrder);

        vm = new ContentBrowserViewModel(this);
        vm->setModels(fsModel, proxy);
        vm->setCurrentDir(QDir::homePath());

        // Views
        auto* splitter = new QSplitter(this);
        auto* leftPanel = new QWidget(this);
        auto* rightPanel = new QWidget(this);
        splitter->addWidget(leftPanel);
        splitter->addWidget(rightPanel);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 2);
        setCentralWidget(splitter);

        // Left panel
        auto* leftLayout = new QVBoxLayout(leftPanel);
        filterEdit = new QLineEdit(leftPanel);
        filterEdit->setPlaceholderText("Filter (e.g. *.png;*.jpg or 'report')");
        tree = new QTreeView(leftPanel);
        tree->setModel(fsModel);
        tree->setHeaderHidden(true);
        tree->setRootIndex(fsModel->index(QDir::homePath()));
        tree->setAnimated(true);
        tree->setExpandsOnDoubleClick(true);
        tree->setSelectionMode(QAbstractItemView::SingleSelection);
        leftLayout->addWidget(filterEdit);
        leftLayout->addWidget(tree);

        // Right panel
        auto* rightLayout = new QVBoxLayout(rightPanel);
        table = new QTableView(rightPanel);
        table->setModel(proxy);
        table->setSortingEnabled(true);
        table->horizontalHeader()->setStretchLastSection(true);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setShowGrid(false);
        table->verticalHeader()->setVisible(false);

        auto* info = new QWidget(rightPanel);
        auto* infoLayout = new QHBoxLayout(info);
        iconLabel = new QLabel(info);
        iconLabel->setFixedSize(64, 64);
        iconLabel->setAlignment(Qt::AlignCenter);
        metaLabel = new QLabel(info);
        metaLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        infoLayout->addWidget(iconLabel);
        infoLayout->addWidget(metaLabel, 1);

        rightLayout->addWidget(table, 1);
        rightLayout->addWidget(info, 0);

        // Toolbar
        auto* tb = addToolBar("Actions");
        upAction = tb->addAction(style()->standardIcon(QStyle::SP_ArrowUp), "Up");
        homeAction = tb->addAction(style()->standardIcon(QStyle::SP_DirHomeIcon), "Home");
        openAction = tb->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton), "Open");

        statusBar();

        // Wiring
        connect(filterEdit, &QLineEdit::textChanged, vm, &ContentBrowserViewModel::setFilterText);
        connect(vm, &ContentBrowserViewModel::statusTextChanged, this, [this](const QString& s) { statusBar()->showMessage(s); });
        connect(tree->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex& current, const QModelIndex&) 
            {
                const QString path = fsModel->fileInfo(current).absoluteFilePath();
                vm->setCurrentDir(path);
                const QModelIndex proxyRoot = proxy->mapFromSource(current);
                table->setRootIndex(proxyRoot);
                updatePreview(QModelIndex());
            });

        connect(table, &QTableView::doubleClicked, this,
            [this](const QModelIndex& proxyIndex) 
            {
                const QModelIndex srcIndex = proxy->mapToSource(proxyIndex);
                const QFileInfo fi = fsModel->fileInfo(srcIndex);
                if (fi.isDir()) {
                    tree->setCurrentIndex(srcIndex);
                    tree->expand(srcIndex);
                }
                else {
                    vm->openPath(fi.absoluteFilePath());
                }
            });

        connect(table->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex& current, const QModelIndex&) 
            {
                updatePreview(current);
            });

        connect(upAction, &QAction::triggered, this, [this] 
            {
                const QModelIndex current = tree->currentIndex();
                const QModelIndex parent = current.parent();
                if (parent.isValid()) 
                {
                    tree->setCurrentIndex(parent);
                    tree->expand(parent);
                }
            });
        connect(homeAction, &QAction::triggered, this, [this] 
            {
                const QModelIndex home = fsModel->index(QDir::homePath());
                tree->setCurrentIndex(home);
                tree->expand(home);
            });
        connect(openAction, &QAction::triggered, this, [this] 
            {
                const QModelIndex current = table->currentIndex();
                if (!current.isValid()) 
                    return;

                const QFileInfo fi = fsModel->fileInfo(proxy->mapToSource(current));
                if (fi.exists() && fi.isFile())
                    QDesktopServices::openUrl(QUrl::fromLocalFile(fi.absoluteFilePath()));
            });

        // Initialize
        const QModelIndex home = fsModel->index(QDir::homePath());
        tree->setCurrentIndex(home);
        tree->expand(home);
        table->setRootIndex(proxy->mapFromSource(home));
        updatePreview(QModelIndex());
    }

private:
    void updatePreview(const QModelIndex& proxyIndex) {
        const QModelIndex srcIndex = proxy->mapToSource(proxyIndex);
        const QFileInfo fi = fsModel->fileInfo(srcIndex);
        const QIcon icon = fsModel->fileIcon(srcIndex);
        iconLabel->setPixmap(icon.pixmap(64, 64));

        const QString type = fi.isDir() ? "Directory" : "File";

        // Qt 6: format with QLocale instead of Qt::DefaultLocaleShortDate
        const QString modifiedStr = QLocale().toString(
            fi.lastModified().toLocalTime(), QLocale::ShortFormat);

        const QString meta = QString(
            "<b>%1</b><br/>Type: %2<br/>Size: %3 bytes<br/>Modified: %4<br/>Path: %5")
            .arg(fi.fileName())
            .arg(type)
            .arg(QString::number(fi.isDir() ? 0 : static_cast<qlonglong>(fi.size())))
            .arg(modifiedStr)
            .arg(fi.absoluteFilePath());

        metaLabel->setText(meta);
    }

private:
    // Models/ViewModel
    QFileSystemModel* fsModel{ nullptr };
    FileFilterProxyModel* proxy{ nullptr };
    ContentBrowserViewModel* vm{ nullptr };

    // Views
    QTreeView* tree{ nullptr };
    QTableView* table{ nullptr };
    QLineEdit* filterEdit{ nullptr };
    QLabel* iconLabel{ nullptr };
    QLabel* metaLabel{ nullptr };

    // Actions
    QAction* upAction{ nullptr };
    QAction* homeAction{ nullptr };
    QAction* openAction{ nullptr };
};