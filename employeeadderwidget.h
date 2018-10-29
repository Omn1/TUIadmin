#ifndef EMPLOYEEADDERWIDGET_H
#define EMPLOYEEADDERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "jsonsender.h"

namespace Ui {
class EmployeeAdderWidget;
}

class EmployeeAdderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeAdderWidget(QWidget *parent = nullptr);
    ~EmployeeAdderWidget();

    void setInitialPermissions();
    void updateColors();
    void updateColor(QPushButton * button);

    QJsonObject getInfo();
public slots:
    void onPermissionButtonClicked();
    void onSendButtonClicked();
private:
    Ui::EmployeeAdderWidget *ui;
    JsonSender *jsonSender;
};

#endif // EMPLOYEEADDERWIDGET_H
