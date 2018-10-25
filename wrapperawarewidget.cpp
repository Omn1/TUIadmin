#include "wrapperawarewidget.h"

WrapperAwareWidget::WrapperAwareWidget(QWidget *parent)
    : QWidget(parent)
{

}

QHBoxLayout *WrapperAwareWidget::getWrapperLayout() const
{
    return wrapperLayout;
}

void WrapperAwareWidget::setWrapperLayout(QHBoxLayout *value)
{
    wrapperLayout = value;
}

void WrapperAwareWidget::fillWrapper()
{

}
