#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentDialog(const Student &student, QWidget *parent = nullptr);
    Student getStudent() const;

private:
    void setupUI();
    void populateFields(const Student &s);
    bool validate();

    Student m_student;

    QLineEdit    *m_firstNameEdit;
    QLineEdit    *m_lastNameEdit;
    QSpinBox     *m_ageSpinBox;
    QComboBox    *m_facultyCombo;
    QLineEdit    *m_specialtyEdit;
    QDoubleSpinBox *m_gpaSpinBox;
    QLineEdit    *m_emailEdit;
    QComboBox    *m_statusCombo;
    QLabel       *m_idLabel;
};

#endif // STUDENTDIALOG_H
