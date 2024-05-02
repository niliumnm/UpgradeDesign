#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<learnwindow.h>
#include<QLabel>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QTextCharFormat>
#include<QTextCursor>
#include<QTextBlock>
#include<qdebug.h>
#include<QFileDialog>
#include<QUrl>
#include<QImageReader>
#include<QFontComboBox>
#include<QSpinBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labelStatus=new QLabel(this);
    labelStatus->setText("就绪");
    ui->statusBar->addWidget(labelStatus);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./mydatabase.db");

    if(db.open())
    {
        QSqlQuery query;
        query.exec("CREATE TABLE mytable (id INTEGER PRIMARY KEY, name TEXT)");
        query.exec("INSERT INTO mytable VALUES(1, 'John')");
        query.exec("SELECT id, name FROM mytable");
        while(query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            qDebug() << id << name;
        }
    }

    db.close();



    ui->toolBar_3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolBar_3->setMaximumHeight(25);



    /*
     * 添加字体选择框和字号选择
    */
    // 创建水平布局
    QSpinBox*spinbox;
    spinbox=new QSpinBox(this);
    spinbox->setRange(1,100);

    QHBoxLayout* innerHlayout = new QHBoxLayout;
    QVBoxLayout*outerVlayout=new QVBoxLayout;


    // 创建字体选择框
    fontComboBox = new QFontComboBox(this);
    fontComboBox->setMaximumWidth(100);

    // 将字体选择框添加到水平布局
    QLabel*fontSizeLab=new QLabel("字号");
    innerHlayout->addWidget(fontSizeLab);
    innerHlayout->addWidget(spinbox);
    outerVlayout->addLayout(innerHlayout);
    outerVlayout->addWidget(fontComboBox);


    // 创建一个 QWidget 对象，将水平布局放置在其中
    QWidget* fontspinWidget = new QWidget(this);
    fontspinWidget->setLayout(outerVlayout);

    // 将 QWidget 对象添加到工具栏
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(fontspinWidget);


    // 当字体选择框的字体发生变化时，将当前选中文字的字体设置为选择框的当前字体
    connect(fontComboBox,&QFontComboBox::currentFontChanged, this, &MainWindow::on_FontComboChanged);
    //bug 有问题 自动改字体
    connect(ui->textEdit,&QTextEdit::cursorPositionChanged,this,&MainWindow::do_changeComboFont);



    // 设置图标大小
    QIcon icon = ui->actRandomList->icon();
    icon = icon.pixmap(QSize(32 , 32));  // 设置为 32x32 的图标大小
    ui->actRandomList->setIcon(icon);

}

void MainWindow::mergeFormat(QTextCharFormat fmt)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(fmt);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLearnMode_clicked()
{
    QMainWindow *newWid=new LearnWindow;
    newWid->show();
}

bool MainWindow::boldcheck(){
    QTextCursor cursor = ui->textEdit->textCursor();
    QFont font = cursor.charFormat().font();
    bool isBold = font.bold();
    return isBold;
}

bool MainWindow::italiccheck()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QFont font = cursor.charFormat().font();
    bool isItalic = font.italic();
    return isItalic;
}

bool MainWindow::underlinecheck()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QFont font = cursor.charFormat().font();
    bool isUnderline = font.underline();
    return isUnderline;
}
void MainWindow::on_actBold_triggered()
{
    bool bold=boldcheck();
    QTextCharFormat fmt;
    fmt.setFontWeight(!bold ? QFont::Bold : QFont::Normal);
    mergeFormat(fmt);
}


void MainWindow::on_actItalic_triggered()
{
    bool italic=italiccheck();
    QTextCharFormat fmt;
    fmt.setFontItalic(!italic ? true : false);
    mergeFormat(fmt);
}


void MainWindow::on_actUnderline_triggered()
{
    bool underline=underlinecheck();
    QTextCharFormat fmt;
    fmt.setFontUnderline(!underline ? true : false);
    mergeFormat(fmt);
}


void MainWindow::on_actAlignL_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignLeft);
}


void MainWindow::on_actAlignR_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignRight);
}


void MainWindow::on_actAlignC_triggered()
{
    ui->textEdit->setAlignment(Qt::AlignCenter);
}


void MainWindow::on_actAddList_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    cursor.insertList(listFormat);

    cursor.insertText("one");
    cursor.insertText("\ntwo");
    cursor.insertText("\nthree");
}


void MainWindow::on_actRandomList_triggered()
{


    // 获取当前光标
    QTextCursor cursor = ui->textEdit->textCursor();

    // 创建无序列表
    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDisc);  // 设置为无序列表风格
    cursor.insertList(listFormat);

    // 或者创建有序列表
    // QTextListFormat listFormat;
    // listFormat.setStyle(QTextListFormat::ListDecimal);  // 设置为有序列表风格
    // cursor.insertList(listFormat);

    // 在列表中插入列表项
    cursor.insertText("列表项1");
    cursor.insertBlock();
    cursor.insertText("列表项2");
    cursor.insertBlock();
    cursor.insertText("列表项3");

    // 将 QTextEdit 设置为只读，以防止用户编辑列表
    // textEdit->setReadOnly(true);
}

void MainWindow::actH(int size)
{
    /*
     * 问题：当选中多个段落的时候只会把第一个设为h1
     * 光标没有回退
     *
    */
    // 获取当前光标
    QTextCursor cursor = ui->textEdit->textCursor();
    QString sizeStr = QString::number(size);
    // 获取标题文本
    QString titleText =  cursor.block().text();
    // 获取光标所在段落的起始位置和结束位置
    int startPos = cursor.block().position();
    int endPos = startPos + cursor.block().length();

    cursor.setPosition(startPos);

    // 选中段落的文本
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endPos - startPos);

    // 删除选中的文本
    cursor.removeSelectedText();


    // 构建带有 <h1> 标签的文本
    QString htmlText = "<h"+sizeStr+">" + titleText + "</h"+sizeStr+"><br>";


    // 在光标位置插入标题文本（带有 HTML 标签）
    cursor.insertHtml(htmlText);



    // // 获取选中文字的字符格式
    // QTextCharFormat format = cursor.charFormat();

    // // 设置字体
    // QFont font("Arial", 12, QFont::Bold);  // 设置为 Arial 字体，大小为 12，加粗
    // font.setPixelSize(36);
    // format.setFont(font);

    // // 设置颜色
    // QColor color(Qt::red);  // 设置为红色
    // format.setForeground(color);

    // // 应用新的字符格式到选中文字
    // cursor.mergeCharFormat(format);
}


void MainWindow::on_actH1_triggered()
{

    // static int size=1;
    // actH(size++);
    // qDebug()<<QString::number(size-1);
    actH(1);
}


void MainWindow::on_actH2_triggered()
{
    actH(2);
}


void MainWindow::on_actH3_triggered()
{
    actH(3);
}


void MainWindow::on_actH4_triggered()
{
    actH(4);
}


void MainWindow::on_actH5_triggered()
{
    actH(5);
}


void MainWindow::on_actH6_triggered()
{
    /*
     * 问题：当选中多个段落的时候只会把第一个设为h1
     * 光标没有回退
     *
    */
    // 获取当前光标
    QTextCursor cursor = ui->textEdit->textCursor();
    // 获取标题文本
    QString titleText =  cursor.block().text();
    // 获取光标所在段落的起始位置和结束位置
    int startPos = cursor.block().position();
    int endPos = startPos + cursor.block().length();

    cursor.setPosition(startPos);

    // 选中段落的文本
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endPos - startPos);

    // 删除选中的文本
    cursor.removeSelectedText();


    // 构建带有 <h1> 标签的文本
    QString htmlText = "<p>"+ titleText + "</p><br>";


    // 在光标位置插入标题文本（带有 HTML 标签）
    cursor.insertHtml(htmlText);
}


void MainWindow::on_actAddImg_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                ".", tr(
                                                   "JPEG (*.jpg *jpeg)\n"
                                                   "Bitmap Files (*.bmp)\n"
                                                   "PNG (*.png)\n"
                                                   "GIF (*.gif)\n"
                                                   "GIF (*.*)\n"
                                                   ));
    // 构建包含图片的 HTML
    QString html = "<img src=\"" + file + "\">";
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertHtml(html);



    /*

    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                ".", tr("Bitmap Files (*.bmp)\n"
                                                   "JPEG (*.jpg *jpeg)\n"
                                                   "GIF (*.gif)\n"
                                                   "PNG (*.png)\n"));
    QUrl Uri ( QString ( "file://%1" ).arg ( file ) );
    QImage image = QImageReader ( file ).read();

    QTextDocument * textDocument = ui->textEdit->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);
*/
}


void MainWindow::on_actOutput_triggered()
{

    // 设置富文本内容到 QTextEdit

    // 获取富文本内容的 HTML 表示
    QString html = ui->textEdit->toHtml();

    // 创建文件并写入富文本内容
    QString fileName = "output.ext";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << html;
        file.close();
    }
}


void MainWindow::on_actInput_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                ".", tr("Bitmap Files (*.ext)\n"
                                                   ));

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString html = stream.readAll();
        file.close();

        // 将读取的富文本内容设置到 QTextEdit
        ui->textEdit->setHtml(html);
    }
}

void MainWindow::on_FontComboChanged(const QFont &f)
{
    if(dontChangeFont==false){
        // 获取当前 QTextEdit 的文本光标
        QTextCursor cursor = ui->textEdit->textCursor();
        QTextCharFormat fmt;
        fmt.setFont(f);
        cursor.mergeCharFormat(fmt);
    }else {
        dontChangeFont=false;
    }



}

void MainWindow::do_changeComboFont()
{
    //解决改变combBox又触发改变字体的情况
    dontChangeFont=true;
    // 获取当前 QTextEdit 的文本光标
    QTextCursor cursor = ui->textEdit->textCursor();

    // 获取光标所在位置的文本属性
    QTextCharFormat format = cursor.charFormat();

    // 获取字体
    QFont font = format.font();


    fontComboBox->setCurrentFont(font);
}


void MainWindow::on_actTable_triggered()
{
    QTextTableFormat tableFormat;
    tableFormat.setBorderCollapse(true);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    auto *table = ui->textEdit->textCursor().insertTable(2, 1, tableFormat);
}

