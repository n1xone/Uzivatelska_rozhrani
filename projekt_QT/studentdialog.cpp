#include "studentdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFrame>

StudentDialog::StudentDialog(const Student &student, QWidget *parent)
    : QDialog(parent)
    , m_student(student)
{
    setMinimumWidth(480);
    setModal(true);
    setupUI();
    populateFields(student);
}

void StudentDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 24, 28, 20);
    mainLayout->setSpacing(16);

    // Header
    QWidget *header = new QWidget();
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(0, 0, 0, 0);

    QLabel *emoji = new QLabel("🧑‍🎓");
    emoji->setStyleSheet("font-size: 32px;");
    hl->addWidget(emoji);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    QLabel *titleLabel = new QLabel(windowTitle().isEmpty() ? "Student" : windowTitle());
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #d0d0ff;");
    m_idLabel = new QLabel();
    m_idLabel->setStyleSheet("font-size: 11px; color: #4a4a80;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(m_idLabel);
    hl->addLayout(titleLayout);
    hl->addStretch();
    mainLayout->addWidget(header);

    // Divider
    QFrame *div = new QFrame();
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("color: #2a2a45;");
    mainLayout->addWidget(div);

    // ── Personal Info group ──
    QGroupBox *personalGroup = new QGroupBox("Personal Information");
    QFormLayout *personalForm = new QFormLayout(personalGroup);
    personalForm->setContentsMargins(16, 20, 16, 16);
    personalForm->setSpacing(10);
    personalForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_firstNameEdit = new QLineEdit();
    m_firstNameEdit->setPlaceholderText("e.g. Alice");
    m_firstNameEdit->setMaxLength(60);
    personalForm->addRow("First Name *", m_firstNameEdit);

    m_lastNameEdit = new QLineEdit();
    m_lastNameEdit->setPlaceholderText("e.g. Novak");
    m_lastNameEdit->setMaxLength(60);
    personalForm->addRow("Last Name *", m_lastNameEdit);

    m_ageSpinBox = new QSpinBox();
    m_ageSpinBox->setRange(16, 80);
    m_ageSpinBox->setSuffix(" years");
    personalForm->addRow("Age *", m_ageSpinBox);

    m_emailEdit = new QLineEdit();
    m_emailEdit->setPlaceholderText("student@university.cz");
    personalForm->addRow("Email", m_emailEdit);

    mainLayout->addWidget(personalGroup);

    // ── Academic Info group ──
    QGroupBox *academicGroup = new QGroupBox("Academic Information");
    QFormLayout *academicForm = new QFormLayout(academicGroup);
    academicForm->setContentsMargins(16, 20, 16, 16);
    academicForm->setSpacing(10);
    academicForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_facultyCombo = new QComboBox();
    m_facultyCombo->setEditable(true);
    m_facultyCombo->addItems({
        "FIT - Faculty of Information Technology",
        "FEI - Faculty of Electrical Engineering",
        "FST - Faculty of Mechanical Engineering",
        "FMT - Faculty of Mathematics",
        "FEK - Faculty of Economics",
        "FAI - Faculty of Applied Informatics",
        "FVHE - Faculty of Veterinary Hygiene"
    });
    m_facultyCombo->lineEdit()->setPlaceholderText("Select or type faculty...");
    academicForm->addRow("Faculty *", m_facultyCombo);

    m_specialtyEdit = new QLineEdit();
    m_specialtyEdit->setPlaceholderText("e.g. Computer Science");
    academicForm->addRow("Specialty *", m_specialtyEdit);

    m_gpaSpinBox = new QDoubleSpinBox();
    m_gpaSpinBox->setRange(0.0, 4.0);
    m_gpaSpinBox->setSingleStep(0.1);
    m_gpaSpinBox->setDecimals(1);
    m_gpaSpinBox->setPrefix("GPA ");
    academicForm->addRow("GPA (0–4.0)", m_gpaSpinBox);

    m_statusCombo = new QComboBox();
    m_statusCombo->addItems({"Active", "Graduated", "Suspended"});
    academicForm->addRow("Status *", m_statusCombo);

    mainLayout->addWidget(academicGroup);

    // ── Buttons ──
    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, this);
    buttons->button(QDialogButtonBox::Ok)->setText("💾  Save");
    buttons->button(QDialogButtonBox::Ok)->setDefault(true);
    buttons->button(QDialogButtonBox::Cancel)->setText("✕  Cancel");

    connect(buttons, &QDialogButtonBox::accepted, this, [this]() {
        if (validate()) accept();
    });
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttons);
}

void StudentDialog::populateFields(const Student &s)
{
    m_idLabel->setText(QString("ID: #%1").arg(s.id));
    m_firstNameEdit->setText(s.firstName);
    m_lastNameEdit->setText(s.lastName);
    m_ageSpinBox->setValue(s.age > 0 ? s.age : 18);
    m_emailEdit->setText(s.email);
    m_facultyCombo->setCurrentText(s.faculty.isEmpty() ? "" : s.faculty);
    m_specialtyEdit->setText(s.specialty);
    m_gpaSpinBox->setValue(s.gpa);
    int idx = m_statusCombo->findText(s.status);
    if (idx >= 0) m_statusCombo->setCurrentIndex(idx);
    m_firstNameEdit->setFocus();
}

bool StudentDialog::validate()
{
    if (m_firstNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "First name is required.");
        m_firstNameEdit->setFocus();
        return false;
    }
    if (m_lastNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Last name is required.");
        m_lastNameEdit->setFocus();
        return false;
    }
    if (m_specialtyEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Specialty is required.");
        m_specialtyEdit->setFocus();
        return false;
    }
    return true;
}

Student StudentDialog::getStudent() const
{
    Student s;
    s.id        = m_student.id;
    s.firstName = m_firstNameEdit->text().trimmed();
    s.lastName  = m_lastNameEdit->text().trimmed();
    s.age       = m_ageSpinBox->value();
    s.email     = m_emailEdit->text().trimmed();
    s.faculty   = m_facultyCombo->currentText().trimmed();
    s.specialty = m_specialtyEdit->text().trimmed();
    s.gpa       = m_gpaSpinBox->value();
    s.status    = m_statusCombo->currentText();
    return s;
}
