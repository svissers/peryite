#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QWidget>

namespace Ui {
class Visualization;
}

class Visualization : public QWidget
{
    Q_OBJECT

public:
    explicit Visualization(QWidget *parent = 0);
    ~Visualization();

private:
    Ui::Visualization *ui;
};

#endif // VISUALIZATION_H
