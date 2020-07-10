#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DataChartViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QJsonObject& layout, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
