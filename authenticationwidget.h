#ifndef AUTHENTICATIONWIDGET_H
#define AUTHENTICATIONWIDGET_H

#include <QWidget>
#include "jsondownloader.h"
#include "jsonsender.h"

namespace Ui {
class AuthenticationWidget;
}

class AuthenticationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AuthenticationWidget(QWidget *parent = nullptr);
    ~AuthenticationWidget();
signals:
    void authenticationPassed();
public slots:
    void onLoginButtonPressed();
    void onLoggedIn(bool success);
private:
    Ui::AuthenticationWidget *ui;
    JsonSender *jsonSender;
};

#endif // AUTHENTICATIONWIDGET_H
