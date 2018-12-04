#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <QStringListModel>
#include "lexique.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actioncharger_lexique_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    QString currentWord = "";
    Lexique* lexique;
    QStringList results;
    QStringListModel *model;
    QString jall_out_pas;
    std::vector<int> resultArray;

    bool isInJall = false;
};

#endif // MAINWINDOW_H
