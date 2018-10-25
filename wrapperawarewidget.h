#ifndef WRAPPERAWAREWIDGET_H
#define WRAPPERAWAREWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

class WrapperAwareWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WrapperAwareWidget(QWidget *parent = nullptr);

    QHBoxLayout *getWrapperLayout() const;
    void setWrapperLayout(QHBoxLayout *value);
    virtual void fillWrapper();
signals:

public slots:

protected:
    QHBoxLayout *wrapperLayout;
};

#endif // WRAPPERAWAREWIDGET_H
