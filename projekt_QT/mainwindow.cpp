#include "mainwindow.h"
#include "studentdialog.h"
#include "aboutdialog.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QHeaderView>
#include <QSplitter>
#include <QFrame>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QDateTime>
#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_filterStatus("All")
{
    setWindowTitle("🎓 Student Manager Pro");
    setMinimumSize(640, 480);
    resize(1280, 780);

    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    applyStyleSheet();
    loadSampleData();
    refreshTable();
    updateStats();

    // Keyboard shortcuts
    new QShortcut(QKeySequence("Ctrl+N"), this, SLOT(addStudent()));
    new QShortcut(QKeySequence("Delete"),  this, SLOT(deleteSelectedStudents()));
    new QShortcut(QKeySequence("F2"),      this, SLOT(editStudent()));
    new QShortcut(QKeySequence("Ctrl+F"),  m_searchEdit, SLOT(setFocus()));
}

MainWindow::~MainWindow() {}

// ─── UI Setup ────────────────────────────────────────────────────────────────

void MainWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Sidebar ──
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setMinimumWidth(160);
    sidebar->setMaximumWidth(220);
    sidebar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(16, 24, 16, 24);
    sideLayout->setSpacing(8);

    QLabel *appIcon = new QLabel("🎓");
    appIcon->setObjectName("appIcon");
    appIcon->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(appIcon);

    QLabel *appTitle = new QLabel("Student\nManager");
    appTitle->setObjectName("appTitle");
    appTitle->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(appTitle);

    QLabel *appVersion = new QLabel("v1.0  by ZUR0037");
    appVersion->setObjectName("appVersion");
    appVersion->setAlignment(Qt::AlignCenter);
    sideLayout->addSpacing(4);
    sideLayout->addWidget(appVersion);

    // Divider
    QFrame *div1 = new QFrame(); div1->setFrameShape(QFrame::HLine);
    div1->setObjectName("divider");
    sideLayout->addSpacing(8);
    sideLayout->addWidget(div1);
    sideLayout->addSpacing(8);

    // Stats
    auto makeStatWidget = [&](const QString &emoji, const QString &label) -> QWidget* {
        QWidget *w = new QWidget();
        w->setObjectName("statCard");
        QVBoxLayout *vl = new QVBoxLayout(w);
        vl->setContentsMargins(12, 10, 12, 10);
        vl->setSpacing(2);
        QLabel *val = new QLabel(emoji + "  —");
        val->setObjectName("statValue");
        QLabel *lbl = new QLabel(label);
        lbl->setObjectName("statLabel");
        vl->addWidget(val);
        vl->addWidget(lbl);
        return w;
    };

    // Store references
    auto *totalCard = makeStatWidget("📋", "Total Students");
    m_totalLabel = totalCard->findChild<QLabel*>("statValue");
    sideLayout->addWidget(totalCard);

    auto *activeCard = makeStatWidget("✅", "Active");
    m_activeLabel = activeCard->findChild<QLabel*>("statValue");
    sideLayout->addWidget(activeCard);

    auto *gradCard = makeStatWidget("🏆", "Graduated");
    m_graduatedLabel = gradCard->findChild<QLabel*>("statValue");
    sideLayout->addWidget(gradCard);

    auto *gpaCard = makeStatWidget("⭐", "Average GPA");
    m_avgGpaLabel = gpaCard->findChild<QLabel*>("statValue");
    sideLayout->addWidget(gpaCard);

    sideLayout->addStretch();

    QLabel *dateLabel = new QLabel(QDateTime::currentDateTime().toString("dd.MM.yyyy"));
    dateLabel->setObjectName("dateLabel");
    dateLabel->setAlignment(Qt::AlignCenter);
    sideLayout->addWidget(dateLabel);

    mainLayout->addWidget(sidebar);

    // ── Right content area ──
    QWidget *content = new QWidget();
    content->setObjectName("content");
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(16, 12, 16, 10);
    contentLayout->setSpacing(8);

    // Header row
    QWidget *headerRow = new QWidget();
    headerRow->setObjectName("headerRow");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerRow);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(8);

    QLabel *pageTitle = new QLabel("Students Database");
    pageTitle->setObjectName("pageTitle");
    pageTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    headerLayout->addWidget(pageTitle);
    headerLayout->addStretch(1);

    // Search
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("🔍  Search...");
    m_searchEdit->setObjectName("searchEdit");
    m_searchEdit->setMinimumWidth(100);
    m_searchEdit->setMaximumWidth(300);
    m_searchEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::searchStudents);
    headerLayout->addWidget(m_searchEdit, 2);

    // Filter
    m_statusFilter = new QComboBox();
    m_statusFilter->setObjectName("statusFilter");
    m_statusFilter->addItems({"All", "Active", "Graduated", "Suspended"});
    m_statusFilter->setMinimumWidth(90);
    m_statusFilter->setMaximumWidth(150);
    m_statusFilter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect(m_statusFilter, &QComboBox::currentTextChanged, this, &MainWindow::filterByStatus);
    headerLayout->addWidget(m_statusFilter);

    contentLayout->addWidget(headerRow);

    // Action buttons row
    QWidget *actionsRow = new QWidget();
    QHBoxLayout *actLayout = new QHBoxLayout(actionsRow);
    actLayout->setContentsMargins(0, 0, 0, 0);
    actLayout->setSpacing(6);

    auto makeBtn = [&](const QString &text, const QString &objName, const QString &tip) -> QPushButton* {
        QPushButton *btn = new QPushButton(text);
        btn->setObjectName(objName);
        btn->setToolTip(tip);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        return btn;
    };

    QPushButton *addBtn  = makeBtn("＋  Add",    "btnAdd",    "Add new student (Ctrl+N)");
    QPushButton *editBtn = makeBtn("✏  Edit",    "btnEdit",   "Edit selected (F2)");
    QPushButton *delBtn  = makeBtn("🗑  Delete",  "btnDelete", "Delete selected (Del)");

    connect(addBtn,  &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editStudent);
    connect(delBtn,  &QPushButton::clicked, this, &MainWindow::deleteSelectedStudents);

    actLayout->addWidget(addBtn);
    actLayout->addWidget(editBtn);
    actLayout->addWidget(delBtn);
    actLayout->addStretch();

    m_countLabel = new QLabel("0 records");
    m_countLabel->setObjectName("countLabel");
    actLayout->addWidget(m_countLabel);

    contentLayout->addWidget(actionsRow);

    // Table
    setupTable();
    contentLayout->addWidget(m_table);

    mainLayout->addWidget(content);
}

void MainWindow::setupTable()
{
    m_table = new QTableWidget();
    m_table->setObjectName("mainTable");
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels({"#", "First Name", "Last Name", "Age", "Faculty", "Specialty", "GPA", "Status"});
    // All columns stretch, last one takes remaining space
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSortIndicatorShown(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setShowGrid(false);
    m_table->setFocusPolicy(Qt::StrongFocus);
    m_table->setWordWrap(false);
    m_table->verticalHeader()->setDefaultSectionSize(42);
    m_table->setColumnWidth(0, 46);   // #  - fixed
    m_table->setColumnWidth(3, 52);   // Age - fixed
    m_table->setColumnWidth(6, 60);   // GPA - fixed

    connect(m_table, &QTableWidget::itemDoubleClicked, [this](QTableWidgetItem *) {
        editStudent();
    });
    connect(m_table->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::sortByColumn);
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onSelectionChanged);
}

void MainWindow::setupMenuBar()
{
    // File menu
    QMenu *fileMenu = menuBar()->addMenu("&File");
    m_importAction = fileMenu->addAction("📂  Import from JSON...");
    connect(m_importAction, &QAction::triggered, this, &MainWindow::importFromJson);
    m_exportAction = fileMenu->addAction("💾  Export to JSON...");
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportToJson);
    fileMenu->addSeparator();
    QAction *clearAction = fileMenu->addAction("🗑  Clear All Data");
    connect(clearAction, &QAction::triggered, this, &MainWindow::clearAll);
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("❌  Exit");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    // Students menu
    QMenu *studMenu = menuBar()->addMenu("&Students");
    m_addAction = studMenu->addAction("➕  Add Student\tCtrl+N");
    connect(m_addAction, &QAction::triggered, this, &MainWindow::addStudent);
    m_editAction = studMenu->addAction("✏  Edit Student\tF2");
    connect(m_editAction, &QAction::triggered, this, &MainWindow::editStudent);
    m_deleteAction = studMenu->addAction("🗑  Delete Student\tDel");
    connect(m_deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedStudents);

    // Help menu
    QMenu *helpMenu = menuBar()->addMenu("&Help");
    QAction *aboutAction = helpMenu->addAction("About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    QAction *aboutQtAction = helpMenu->addAction("About Qt");
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);

}

void MainWindow::setupToolBar()
{
    QToolBar *tb = addToolBar("Main");
    tb->setObjectName("mainToolBar");
    tb->setMovable(false);
    tb->setIconSize(QSize(16, 16));
    tb->setToolButtonStyle(Qt::ToolButtonTextOnly);

    tb->addAction(m_addAction);
    tb->addAction(m_editAction);
    tb->addAction(m_deleteAction);
    tb->addSeparator();
    tb->addAction(m_importAction);
    tb->addAction(m_exportAction);
}

void MainWindow::setupStatusBar()
{
    m_statusLabel = new QLabel("Ready");
    m_statusLabel->setObjectName("statusLabel");
    statusBar()->addWidget(m_statusLabel);
    statusBar()->setObjectName("statusBar");
}

// ─── Data ────────────────────────────────────────────────────────────────────

void MainWindow::loadSampleData()
{
    m_students = {
        {1,  "Alice",   "Novak",    20, "FIT",  "Computer Science",   3.9, "alice@uni.cz",   "Active"},
        {2,  "Tomáš",   "Dvořák",   22, "FEI",  "Electrical Eng.",    3.4, "tomas@uni.cz",   "Active"},
        {3,  "Karolína","Horáková", 23, "FST",  "Mechanical Eng.",    2.8, "karo@uni.cz",    "Active"},
        {4,  "Lukáš",   "Procházka",24, "FIT",  "Software Eng.",      3.7, "lukas@uni.cz",   "Graduated"},
        {5,  "Eva",     "Kratochvíl",21,"FEK",  "Economics",          3.1, "eva@uni.cz",     "Active"},
        {6,  "Martin",  "Veselý",   25, "FIT",  "Cybersecurity",      3.6, "martin@uni.cz",  "Graduated"},
        {7,  "Jana",    "Bláha",    19, "FMT",  "Mathematics",        4.0, "jana@uni.cz",    "Active"},
        {8,  "Petr",    "Šimánek",  22, "FEI",  "Robotics",           2.5, "petr@uni.cz",    "Suspended"},
        {9,  "Tereza",  "Kopecká",  20, "FAI",  "Artificial Intel.",  3.8, "tereza@uni.cz",  "Active"},
        {10, "Ondřej",  "Marek",    23, "FIT",  "Data Science",       3.3, "ondrej@uni.cz",  "Active"},
        {11, "Lenka",   "Fišerová", 21, "FEK",  "Business Admin.",    2.9, "lenka@uni.cz",   "Active"},
        {12, "Radek",   "Tůma",     26, "FST",  "Civil Engineering",  3.5, "radek@uni.cz",   "Graduated"},
    };
    m_filteredStudents = m_students;
}

int MainWindow::getNextId()
{
    int maxId = 0;
    for (const auto &s : m_students)
        maxId = qMax(maxId, s.id);
    return maxId + 1;
}

// ─── Table Refresh ────────────────────────────────────────────────────────────

void MainWindow::refreshTable()
{
    // Build filtered list
    m_filteredStudents.clear();
    for (const auto &s : m_students) {
        bool matchSearch = m_searchText.isEmpty()
            || s.firstName.contains(m_searchText, Qt::CaseInsensitive)
            || s.lastName.contains(m_searchText, Qt::CaseInsensitive)
            || s.faculty.contains(m_searchText, Qt::CaseInsensitive)
            || s.specialty.contains(m_searchText, Qt::CaseInsensitive)
            || s.email.contains(m_searchText, Qt::CaseInsensitive);
        bool matchStatus = (m_filterStatus == "All") || (s.status == m_filterStatus);
        if (matchSearch && matchStatus)
            m_filteredStudents.append(s);
    }

    // Sort
    if (m_sortColumn >= 0) {
        std::sort(m_filteredStudents.begin(), m_filteredStudents.end(), [&](const Student &a, const Student &b) {
            bool less = false;
            switch (m_sortColumn) {
            case 0: less = a.id < b.id; break;
            case 1: less = a.firstName < b.firstName; break;
            case 2: less = a.lastName < b.lastName; break;
            case 3: less = a.age < b.age; break;
            case 4: less = a.faculty < b.faculty; break;
            case 5: less = a.specialty < b.specialty; break;
            case 6: less = a.gpa < b.gpa; break;
            case 7: less = a.status < b.status; break;
            }
            return m_sortAscending ? less : !less;
        });
    }

    m_table->setRowCount(m_filteredStudents.size());
    for (int i = 0; i < m_filteredStudents.size(); ++i)
        setRowFromStudent(i, m_filteredStudents[i]);

    m_countLabel->setText(QString("%1 record%2").arg(m_filteredStudents.size()).arg(m_filteredStudents.size() != 1 ? "s" : ""));
    updateStats();
}

void MainWindow::setRowFromStudent(int row, const Student &s)
{
    auto item = [](const QString &text, Qt::AlignmentFlag align = Qt::AlignVCenter) {
        QTableWidgetItem *it = new QTableWidgetItem(text);
        it->setTextAlignment(align | Qt::AlignLeft);
        return it;
    };

    m_table->setItem(row, 0, item(QString::number(s.id), Qt::AlignCenter));
    m_table->setItem(row, 1, item(s.firstName));
    m_table->setItem(row, 2, item(s.lastName));
    m_table->setItem(row, 3, item(QString::number(s.age), Qt::AlignCenter));
    m_table->setItem(row, 4, item(s.faculty));
    m_table->setItem(row, 5, item(s.specialty));

    QTableWidgetItem *gpaItem = new QTableWidgetItem(QString::number(s.gpa, 'f', 1));
    gpaItem->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    QColor gpaColor = s.gpa >= 3.7 ? QColor(80, 220, 120) :
                      s.gpa >= 3.0 ? QColor(255, 200, 60) :
                                     QColor(255, 100, 80);
    gpaItem->setForeground(gpaColor);
    m_table->setItem(row, 6, gpaItem);

    // Status badge
    QTableWidgetItem *statusItem = new QTableWidgetItem("  " + s.status);
    statusItem->setForeground(statusColor(s.status));
    statusItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_table->setItem(row, 7, statusItem);
}

QColor MainWindow::statusColor(const QString &status)
{
    if (status == "Active")    return QColor(80, 220, 120);
    if (status == "Graduated") return QColor(100, 160, 255);
    if (status == "Suspended") return QColor(255, 100, 80);
    return QColor(180, 180, 180);
}

void MainWindow::updateStats()
{
    int total = m_students.size();
    int active = 0, graduated = 0;
    double totalGpa = 0;
    for (const auto &s : m_students) {
        if (s.status == "Active")    active++;
        if (s.status == "Graduated") graduated++;
        totalGpa += s.gpa;
    }
    double avgGpa = total > 0 ? totalGpa / total : 0.0;

    m_totalLabel->setText(QString("📋  %1").arg(total));
    m_activeLabel->setText(QString("✅  %1").arg(active));
    m_graduatedLabel->setText(QString("🏆  %1").arg(graduated));
    m_avgGpaLabel->setText(QString("⭐  %1").arg(QString::number(avgGpa, 'f', 2)));
}

void MainWindow::updateStatusBar()
{
    m_statusLabel->setText(QString("Total: %1 students | Showing: %2")
        .arg(m_students.size())
        .arg(m_filteredStudents.size()));
}

// ─── CRUD Slots ──────────────────────────────────────────────────────────────

void MainWindow::addStudent()
{
    Student s;
    s.id = getNextId();
    s.age = 18;
    s.gpa = 3.0;
    s.status = "Active";

    StudentDialog dlg(s, this);
    dlg.setWindowTitle("Add New Student");
    if (dlg.exec() == QDialog::Accepted) {
        s = dlg.getStudent();
        m_students.append(s);
        refreshTable();
        updateStatusBar();
        m_statusLabel->setText(QString("✅ Student '%1 %2' added.").arg(s.firstName, s.lastName));
    }
}

void MainWindow::editStudent()
{
    QList<QTableWidgetItem*> selected = m_table->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, "Edit", "Please select a student to edit.");
        return;
    }
    int row = m_table->currentRow();
    if (row < 0 || row >= m_filteredStudents.size()) return;

    Student s = m_filteredStudents[row];
    StudentDialog dlg(s, this);
    dlg.setWindowTitle("Edit Student");
    if (dlg.exec() == QDialog::Accepted) {
        Student updated = dlg.getStudent();
        // Update in main list
        for (auto &ms : m_students) {
            if (ms.id == updated.id) {
                ms = updated;
                break;
            }
        }
        refreshTable();
        m_statusLabel->setText(QString("✏ Student '%1 %2' updated.").arg(updated.firstName, updated.lastName));
    }
}

void MainWindow::deleteStudent()
{
    int row = m_table->currentRow();
    if (row < 0 || row >= m_filteredStudents.size()) return;
    Student s = m_filteredStudents[row];
    int ret = QMessageBox::question(this, "Delete Student",
        QString("Delete '%1 %2'?").arg(s.firstName, s.lastName),
        QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        m_students.erase(std::remove_if(m_students.begin(), m_students.end(),
            [&](const Student &st) { return st.id == s.id; }), m_students.end());
        refreshTable();
        m_statusLabel->setText(QString("🗑 Student deleted."));
    }
}

void MainWindow::deleteSelectedStudents()
{
    QSet<int> toDelete;
    for (auto *item : m_table->selectedItems())
        toDelete.insert(m_filteredStudents[item->row()].id);

    if (toDelete.isEmpty()) return;

    int ret = QMessageBox::question(this, "Delete",
        QString("Delete %1 selected student(s)?").arg(toDelete.size()),
        QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    m_students.erase(std::remove_if(m_students.begin(), m_students.end(),
        [&](const Student &s) { return toDelete.contains(s.id); }), m_students.end());
    refreshTable();
    m_statusLabel->setText(QString("🗑 %1 student(s) deleted.").arg(toDelete.size()));
}

void MainWindow::clearAll()
{
    if (m_students.isEmpty()) return;
    int ret = QMessageBox::warning(this, "Clear All",
        "This will delete ALL student records. Continue?",
        QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        m_students.clear();
        refreshTable();
        m_statusLabel->setText("🗑 All data cleared.");
    }
}

// ─── Search & Filter ─────────────────────────────────────────────────────────

void MainWindow::searchStudents(const QString &text)
{
    m_searchText = text;
    refreshTable();
}

void MainWindow::filterByStatus(const QString &status)
{
    m_filterStatus = status;
    refreshTable();
}

void MainWindow::sortByColumn(int col)
{
    if (m_sortColumn == col)
        m_sortAscending = !m_sortAscending;
    else {
        m_sortColumn = col;
        m_sortAscending = true;
    }
    refreshTable();
}

void MainWindow::onSelectionChanged()
{
    bool has = !m_table->selectedItems().isEmpty();
    m_editAction->setEnabled(has);
    m_deleteAction->setEnabled(has);
}

void MainWindow::tableDoubleClicked(int, int) { editStudent(); }

// ─── Import / Export ─────────────────────────────────────────────────────────

void MainWindow::exportToJson()
{
    QString path = QFileDialog::getSaveFileName(this, "Export to JSON", "students.json", "JSON Files (*.json)");
    if (path.isEmpty()) return;

    QJsonArray arr;
    for (const auto &s : m_students) {
        QJsonObject obj;
        obj["id"]        = s.id;
        obj["firstName"] = s.firstName;
        obj["lastName"]  = s.lastName;
        obj["age"]       = s.age;
        obj["faculty"]   = s.faculty;
        obj["specialty"] = s.specialty;
        obj["gpa"]       = s.gpa;
        obj["email"]     = s.email;
        obj["status"]    = s.status;
        arr.append(obj);
    }

    QFile f(path);
    if (f.open(QIODevice::WriteOnly)) {
        f.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
        m_statusLabel->setText(QString("💾 Exported %1 records to %2").arg(m_students.size()).arg(path));
    }
}

void MainWindow::importFromJson()
{
    QString path = QFileDialog::getOpenFileName(this, "Import JSON", "", "JSON Files (*.json)");
    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    if (!doc.isArray()) {
        QMessageBox::warning(this, "Import", "Invalid JSON format.");
        return;
    }

    int imported = 0;
    for (const auto &val : doc.array()) {
        QJsonObject obj = val.toObject();
        Student s;
        s.id        = obj["id"].toInt(getNextId());
        s.firstName = obj["firstName"].toString();
        s.lastName  = obj["lastName"].toString();
        s.age       = obj["age"].toInt();
        s.faculty   = obj["faculty"].toString();
        s.specialty = obj["specialty"].toString();
        s.gpa       = obj["gpa"].toDouble();
        s.email     = obj["email"].toString();
        s.status    = obj["status"].toString("Active");
        m_students.append(s);
        imported++;
    }

    refreshTable();
    m_statusLabel->setText(QString("📂 Imported %1 records.").arg(imported));
}

// ─── About ───────────────────────────────────────────────────────────────────

void MainWindow::showAbout()
{
    AboutDialog dlg(this);
    dlg.exec();
}

// ─── Style Sheet ─────────────────────────────────────────────────────────────

void MainWindow::applyStyleSheet()
{
    setStyleSheet(R"(
        QMainWindow {
            background: #12121e;
        }

        /* ─── Sidebar ─── */
        #sidebar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a2e, stop:1 #16213e);
            border-right: 1px solid #2a2a45;
        }
        #appIcon {
            font-size: 42px;
            margin-bottom: 4px;
        }
        #appTitle {
            font-size: 18px;
            font-weight: bold;
            color: #c8c8f0;
            letter-spacing: 1px;
        }
        #appVersion {
            font-size: 10px;
            color: #5555aa;
        }
        #divider {
            color: #2a2a45;
            border: none;
            border-top: 1px solid #2a2a45;
        }
        #statCard {
            background: rgba(80, 100, 200, 0.12);
            border: 1px solid rgba(80, 100, 200, 0.2);
            border-radius: 8px;
            margin: 2px 0;
        }
        #statCard:hover {
            background: rgba(80, 100, 200, 0.2);
            border-color: rgba(100, 130, 255, 0.4);
        }
        #statValue {
            font-size: 14px;
            font-weight: bold;
            color: #a0b4ff;
        }
        #statLabel {
            font-size: 10px;
            color: #5555aa;
        }
        #dateLabel {
            font-size: 10px;
            color: #444466;
        }

        /* ─── Content ─── */
        #content {
            background: #12121e;
        }
        #pageTitle {
            font-size: 22px;
            font-weight: bold;
            color: #e0e0ff;
            letter-spacing: 0.5px;
        }

        /* ─── Search & Filter ─── */
        #searchEdit {
            background: #1e1e35;
            border: 1.5px solid #2e2e50;
            border-radius: 8px;
            padding: 7px 14px;
            font-size: 13px;
            color: #c0c0e0;
            selection-background-color: #4060cc;
        }
        #searchEdit:focus {
            border-color: #5070dd;
            background: #22223e;
        }
        #statusFilter {
            background: #1e1e35;
            border: 1.5px solid #2e2e50;
            border-radius: 8px;
            padding: 6px 12px;
            font-size: 13px;
            color: #c0c0e0;
            min-width: 110px;
        }
        #statusFilter:focus { border-color: #5070dd; }
        #statusFilter QAbstractItemView {
            background: #1e1e35;
            border: 1px solid #3a3a60;
            color: #c0c0e0;
            selection-background-color: #4060cc;
        }

        /* ─── Buttons ─── */
        #btnAdd {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3a6de8, stop:1 #5b4de8);
            color: white;
            border: none;
            border-radius: 8px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
        }
        #btnAdd:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #4a7df8, stop:1 #6b5df8);
        }
        #btnAdd:pressed { padding-top: 9px; padding-bottom: 7px; }

        #btnEdit {
            background: #252540;
            color: #90a0ff;
            border: 1.5px solid #3a3a60;
            border-radius: 8px;
            padding: 7px 16px;
            font-size: 13px;
        }
        #btnEdit:hover { background: #303060; border-color: #5060bb; }

        #btnDelete {
            background: #251525;
            color: #ff6070;
            border: 1.5px solid #4a2535;
            border-radius: 8px;
            padding: 7px 16px;
            font-size: 13px;
        }
        #btnDelete:hover { background: #3a1a25; border-color: #aa2535; }

        #countLabel {
            font-size: 12px;
            color: #4a4a70;
            font-style: italic;
        }

        /* ─── Table ─── */
        #mainTable {
            background: #161628;
            alternate-background-color: #1c1c32;
            border: 1px solid #252540;
            border-radius: 10px;
            gridline-color: transparent;
            color: #c8c8e8;
            font-size: 13px;
            selection-background-color: rgba(60, 80, 200, 0.35);
            selection-color: #ffffff;
            outline: none;
        }
        #mainTable::item {
            padding: 4px 8px;
            border-bottom: 1px solid rgba(255,255,255,0.03);
        }
        #mainTable::item:selected {
            background: rgba(60, 80, 200, 0.35);
        }
        #mainTable::item:hover {
            background: rgba(60, 80, 200, 0.15);
        }
        QHeaderView::section {
            background: #1e1e38;
            color: #7080cc;
            border: none;
            border-bottom: 2px solid #3040aa;
            padding: 8px 10px;
            font-size: 12px;
            font-weight: bold;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        QHeaderView::section:hover {
            background: #252550;
            color: #a0b0ff;
        }
        QScrollBar:vertical {
            background: #14142a;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #303060;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover { background: #5060bb; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }

        /* ─── Menu Bar ─── */
        QMenuBar {
            background: #0e0e1e;
            color: #9090c0;
            border-bottom: 1px solid #1e1e38;
            padding: 2px 6px;
            font-size: 13px;
        }
        QMenuBar::item:selected { background: #252550; color: #c0c0ff; border-radius: 4px; }
        QMenu {
            background: #1a1a30;
            color: #c0c0e0;
            border: 1px solid #2e2e50;
            border-radius: 6px;
            padding: 4px;
        }
        QMenu::item { padding: 7px 24px 7px 12px; border-radius: 4px; font-size: 13px; }
        QMenu::item:selected { background: #303060; }
        QMenu::separator { height: 1px; background: #252545; margin: 4px 8px; }

        /* ─── Tool Bar ─── */
        #mainToolBar {
            background: #0e0e1e;
            border-bottom: 1px solid #1e1e38;
            spacing: 4px;
            padding: 3px 8px;
        }
        QToolButton {
            background: transparent;
            color: #8090c0;
            border: none;
            border-radius: 6px;
            padding: 5px 10px;
            font-size: 12px;
        }
        QToolButton:hover { background: #252550; color: #c0c0ff; }
        QToolButton:pressed { background: #303068; }

        /* ─── Status Bar ─── */
        QStatusBar {
            background: #0e0e1e;
            border-top: 1px solid #1e1e38;
            color: #4a4a70;
            font-size: 11px;
            padding: 2px 10px;
        }

        /* ─── Dialogs ─── */
        QDialog {
            background: #12121e;
        }
        QLabel { color: #c0c0e0; font-size: 13px; }
        QLineEdit, QSpinBox, QDoubleSpinBox {
            background: #1e1e35;
            border: 1.5px solid #2e2e50;
            border-radius: 7px;
            padding: 7px 12px;
            color: #d0d0f0;
            font-size: 13px;
            selection-background-color: #4060cc;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #5070dd;
        }
        QComboBox {
            background: #1e1e35;
            border: 1.5px solid #2e2e50;
            border-radius: 7px;
            padding: 7px 12px;
            color: #d0d0f0;
            font-size: 13px;
        }
        QComboBox:focus { border-color: #5070dd; }
        QComboBox QAbstractItemView {
            background: #1e1e35;
            border: 1px solid #3a3a60;
            color: #d0d0f0;
            selection-background-color: #4060cc;
        }
        QPushButton {
            background: #252545;
            color: #a0a0d0;
            border: 1px solid #3a3a60;
            border-radius: 7px;
            padding: 8px 18px;
            font-size: 13px;
            min-width: 80px;
        }
        QPushButton:hover { background: #303060; color: #d0d0ff; border-color: #5060bb; }
        QPushButton:default {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3a6de8, stop:1 #5b4de8);
            color: white;
            border: none;
        }
        QPushButton:default:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #4a7df8, stop:1 #6b5df8);
        }
        QDialogButtonBox QPushButton { min-width: 80px; }
        QGroupBox {
            color: #7080cc;
            border: 1px solid #2a2a50;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 8px;
            font-size: 12px;
            font-weight: bold;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 8px;
            left: 12px;
        }
        QMessageBox {
            background: #12121e;
        }
        QScrollArea { border: none; background: transparent; }
    )");
}
