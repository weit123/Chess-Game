#ifndef SERVERGAMECONFIG_H
#define SERVERGAMECONFIG_H

#include <QDialog>

namespace Ui {
class serverGameConfig;
}

class serverGameConfig : public QDialog
{
    Q_OBJECT

public:
    explicit serverGameConfig(QWidget *parent = nullptr);
    ~serverGameConfig();

signals:
    void gameConfigResult(QString);

public slots:
    void blackClicked();
    void whiteClicked();
    void checkboxChanged(int);
    void timeChanged(int);
    void sendSignal();

private:
    Ui::serverGameConfig *ui;

    bool side;
    bool limitEnable;
    int timeLimit;
};

#endif // SERVERGAMECONFIG_H
