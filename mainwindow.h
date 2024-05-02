#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QLabel>
#include<QTextCharFormat>
#include<QFontComboBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void mergeFormat(QTextCharFormat fmt);
    bool boldcheck();
    bool italiccheck();
    bool underlinecheck();
    ~MainWindow();

private slots:

    void on_btnLearnMode_clicked();

    void on_actBold_triggered();

    void on_actItalic_triggered();

    void on_actUnderline_triggered();

    void on_actAlignL_triggered();

    void on_actAlignR_triggered();

    void on_actAlignC_triggered();

    void on_actAddList_triggered();

    void on_actRandomList_triggered();

    void actH(int size);

    void on_actH1_triggered();

    void on_actH2_triggered();

    void on_actH3_triggered();

    void on_actH4_triggered();

    void on_actH5_triggered();

    void on_actH6_triggered();

    void on_actAddImg_triggered();

    void on_actOutput_triggered();

    void on_actInput_triggered();

    void on_FontComboChanged(const QFont&);

    void do_changeComboFont();

    void on_actTable_triggered();

private:
    Ui::MainWindow *ui;
    QLabel*labelStatus;
    QFontComboBox *fontComboBox;
    bool dontChangeFont=false;
};
#endif // MAINWINDOW_H
