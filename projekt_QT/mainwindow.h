#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QAction>
#include <QMenuBar>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>

struct Student {
    int id;
    QString firstName;
    QString lastName;
    int age;
    QString faculty;
    QString specialty;
    double gpa;
    QString email;
    QString status; // "Active", "Graduated", "Suspended"
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addStudent();
    void editStudent();
    void deleteStudent();
    void deleteSelectedStudents();
    void searchStudents(const QString &text);
    void filterByStatus(const QString &status);
    void showAbout();
    void exportToJson();
    void importFromJson();
    void clearAll();
    void onSelectionChanged();
    void tableDoubleClicked(int row, int col);
    void sortByColumn(int col);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupTable();
    void setupSidebar();
    void loadSampleData();
    void refreshTable();
    void updateStatusBar();
    void updateStats();
    void applyStyleSheet();
    Student getStudentFromRow(int row);
    void setRowFromStudent(int row, const Student &s);
    int getNextId();
    QColor statusColor(const QString &status);

    QVector<Student> m_students;
    QVector<Student> m_filteredStudents;
    QString m_searchText;
    QString m_filterStatus;
    int m_sortColumn = -1;
    bool m_sortAscending = true;

    // UI elements
    QTableWidget *m_table;
    QLineEdit *m_searchEdit;
    QComboBox *m_statusFilter;
    QLabel *m_statusLabel;
    QLabel *m_statsLabel;
    QLabel *m_countLabel;

    // Sidebar stats
    QLabel *m_totalLabel;
    QLabel *m_activeLabel;
    QLabel *m_graduatedLabel;
    QLabel *m_avgGpaLabel;

    // Actions
    QAction *m_addAction;
    QAction *m_editAction;
    QAction *m_deleteAction;
    QAction *m_exportAction;
    QAction *m_importAction;
};

#endif // MAINWINDOW_H
