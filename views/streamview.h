#ifndef STREAMVIEW_H
#define STREAMVIEW_H

#include <QMainWindow>

namespace Ui {
class StreamView;
}

class StreamView : public QMainWindow
{
    Q_OBJECT

public:
    explicit StreamView(QWidget *parent = 0);
    ~StreamView();

private:
    Ui::StreamView *ui;
};

#endif // STREAMVIEW_H
