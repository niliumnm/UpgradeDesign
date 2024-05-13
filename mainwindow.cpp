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
#include<QStandardItem>
#include<QJsonArray>
#include<QJsonDocument>
#include"sql/idgenerator.h"
#include<QItemSelectionModel>
#include<QMessageBox>
#include<iostream>
#include<QTextStream>

const int MAX_UNDO_COUNT = 50;

QSettings*MainWindow::settings=nullptr;

MainWindow::MainWindow(QWidget * parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui -> setupUi(this);


    labelStatus = new QLabel(this);
    labelStatus -> setText("就绪");
    ui -> statusBar -> addWidget(labelStatus);
    ui -> toolBar_3 -> setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui -> toolBar_3 -> setMaximumHeight(25);

    //初始化数据库
    generator = IdGenerator::getInstance();
    sqlUtil = SqlUtil::getInstance();
    sqlUtil -> init();

    /*
     * 添加字体选择框和字号选择
     */
    // 创建水平布局
    QSpinBox * spinbox;
    spinbox = new QSpinBox(this);
    spinbox -> setRange(1, 100);

    QHBoxLayout * innerHlayout = new QHBoxLayout;
    QVBoxLayout * outerVlayout = new QVBoxLayout;

    // 创建字体选择框
    fontComboBox = new QFontComboBox(this);
    fontComboBox -> setMaximumWidth(100);

    // 将字体选择框添加到水平布局
    QLabel * fontSizeLab = new QLabel("字号");
    innerHlayout -> addWidget(fontSizeLab);
    innerHlayout -> addWidget(spinbox);
    outerVlayout -> addLayout(innerHlayout);
    outerVlayout -> addWidget(fontComboBox);

    // 创建一个 QWidget 对象，将水平布局放置在其中
    QWidget * fontspinWidget = new QWidget(this);
    fontspinWidget -> setLayout(outerVlayout);

    // 将 QWidget 对象添加到工具栏
    ui -> toolBar -> addSeparator();
    ui -> toolBar -> addWidget(fontspinWidget);

    // 当字体选择框的字体发生变化时，将当前选中文字的字体设置为选择框的当前字体
    connect(fontComboBox, & QFontComboBox::currentFontChanged, this, & MainWindow::on_FontComboChanged);
    //bug 有问题 自动改字体
    connect(ui -> textEdit, & QTextEdit::cursorPositionChanged, this, & MainWindow::do_changeComboFont);

    // 设置图标大小
    QIcon icon = ui -> actRandomList -> icon();
    icon = icon.pixmap(QSize(32, 32)); // 设置为 32x32 的图标大小
    ui -> actRandomList -> setIcon(icon);

    /*
     * 初始化qtreeView
     */
    // 初始化model
    m_standardItemModel = new QStandardItemModel();
    m_standardItemModel -> setHorizontalHeaderLabels(QStringList(QStringLiteral("名称")));
    ui -> treeView -> setModel(m_standardItemModel);
    ui -> treeView -> expandAll();

    //初始化菜单
    initTreeMenu();


    restoreTree();
    //setTreeTestData();

    // 创建选择模型
    selectionModel = new QItemSelectionModel;
    ui -> treeView -> setSelectionModel(selectionModel);

    //Treeview改变时
    connect(ui -> treeView -> selectionModel(), & QItemSelectionModel::currentChanged, this, do_treeSelectChanged);
    //Textedit改变时
    connect(ui -> textEdit, & QTextEdit::textChanged, this, & MainWindow::do_saveNote);



    QModelIndex firstIndex = ui -> treeView -> model() -> index(0, 0);
    selectionModel -> setCurrentIndex(firstIndex, QItemSelectionModel::Select);

    ui -> treeView -> setDragEnabled(true); //允许拖拽
    ui -> treeView -> setDragDropMode(QAbstractItemView::InternalMove); //拖放模式为移动
    ui -> treeView -> setDropIndicatorShown(true); //显示拖放位置

    // test();

    settings=new QSettings ("./config.ini", QSettings::IniFormat);
    /*
    settings->setValue("General/color", "#7e2598");
    settings->setValue("General/savePath", "D:/test");
    QString str= settings->value(QString("General/savePath")).toString();
    qDebug()<<settings->fileName();
    */
    dataUtil=DataUtil::getInstance();
    dataUtil->start();

    // 1. 设置菜单文本的策略为自定义
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    // 2. 为QTreeWidget的对象指针 tree 创建信号槽，来接受处理右键事件。
    connect(ui->treeView, &QTreeWidget::customContextMenuRequested, this, &MainWindow::OnRightClicked);

    setDebugOutput( "./%1.log", true);


    QFile f(":qdarkstyle/light/lightstyle.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

}

void MainWindow::mergeFormat(QTextCharFormat fmt) {
    QTextCursor cursor = ui -> textEdit -> textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(fmt);
}

MainWindow::~MainWindow() {
    saveTree();
    delete treeMenu;
    delete ui;
}


QSettings *MainWindow::getSettings()
{
    if(!settings){
        settings=new QSettings ("./config.ini", QSettings::IniFormat);
        settings->setValue("General/color", "#7e2598");
        settings->setValue("General/savePath", "D:/test");
    }
    return settings;
}

void MainWindow::on_btnLearnMode_clicked() {
    if(!learnWindow){
        learnWindow=new LearnWindow;
    }
    learnWindow->show();
}

bool MainWindow::boldcheck() {
    QTextCursor cursor = ui -> textEdit -> textCursor();
    QFont font = cursor.charFormat().font();
    bool isBold = font.bold();
    return isBold;
}

bool MainWindow::italiccheck() {
    QTextCursor cursor = ui -> textEdit -> textCursor();
    QFont font = cursor.charFormat().font();
    bool isItalic = font.italic();
    return isItalic;
}

bool MainWindow::underlinecheck() {
    QTextCursor cursor = ui -> textEdit -> textCursor();
    QFont font = cursor.charFormat().font();
    bool isUnderline = font.underline();
    return isUnderline;
}

void MainWindow::saveTreeViewToJson(QStandardItem * item, QJsonObject & jsonObject) {
    // 保存当前节点的文本内容
    QVariant idVariant = item -> data(Qt::UserRole);
    uint64_t id = idVariant.toULongLong();
    QString sId = QString::number(id);
    jsonObject["id"] = sId;
    jsonObject["name"] = item -> text();

    // 检查是否有子节点
    if (item -> rowCount() > 0) {
        QJsonArray childrenArray;

        // 递归保存子节点
        for (int i = 0; i < item -> rowCount(); ++i) {
            QStandardItem * childItem = item -> child(i);
            QJsonObject childObject;
            saveTreeViewToJson(childItem, childObject);
            childrenArray.append(childObject);
        }

        // 将子节点数组添加到当前节点的 JSON 对象中
        jsonObject["children"] = childrenArray;
    }

}

void MainWindow::test() {
    //sqlUtil->setNote(12558299493519310220,"更新成功",SqlUtil::Question,"测试30","测试40");
    QString string;
    int type;
    QString question;
    QString anser;
    QString resId;
    sqlUtil -> getNote(12558299493519310220, string, type, question, anser, resId);
}

// void MainWindow::saveSubTreeToJson(QTreeWidgetItem *item, QJsonObject &jsonObject)
// {
//     // 保存当前节点的文本内容
//     jsonObject["id"] = item->text(0);

//     // 检查是否有子节点
//     if (item->childCount() > 0)
//     {
//         QJsonArray childrenArray;

//         // 递归保存子节点
//         for (int i = 0; i < item->childCount(); ++i)
//         {
//             QTreeWidgetItem* childItem = item->child(i);
//             QJsonObject childObject;
//             saveSubTreeToJson(childItem, childObject);
//             childrenArray.append(childObject);
//         }

//         // 将子节点数组添加到当前节点的 JSON 对象中
//         jsonObject["children"] = childrenArray;
//     }
// }
void MainWindow::on_actBold_triggered() {
    bool bold = boldcheck();
    QTextCharFormat fmt;
    fmt.setFontWeight(!bold ? QFont::Bold : QFont::Normal);
    mergeFormat(fmt);
}

void MainWindow::on_actItalic_triggered() {
    bool italic = italiccheck();
    QTextCharFormat fmt;
    fmt.setFontItalic(!italic ? true : false);
    mergeFormat(fmt);
}

void MainWindow::on_actUnderline_triggered() {
    bool underline = underlinecheck();
    QTextCharFormat fmt;
    fmt.setFontUnderline(!underline ? true : false);
    mergeFormat(fmt);
}

void MainWindow::on_actAlignL_triggered() {
    ui -> textEdit -> setAlignment(Qt::AlignLeft);
}

void MainWindow::on_actAlignR_triggered() {
    ui -> textEdit -> setAlignment(Qt::AlignRight);
}

void MainWindow::on_actAlignC_triggered() {
    ui -> textEdit -> setAlignment(Qt::AlignCenter);
}

void MainWindow::on_actAddList_triggered() {
    QTextCursor cursor = ui -> textEdit -> textCursor();
    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDecimal);
    cursor.insertList(listFormat);

    cursor.insertText("one");
    cursor.insertText("\ntwo");
    cursor.insertText("\nthree");
}

void MainWindow::on_actRandomList_triggered() {

    // 获取当前光标
    QTextCursor cursor = ui -> textEdit -> textCursor();

    // 创建无序列表
    QTextListFormat listFormat;
    listFormat.setStyle(QTextListFormat::ListDisc); // 设置为无序列表风格
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

void MainWindow::actH(int size) {
    /*
     * 问题：当选中多个段落的时候只会把第一个设为h1
     * 光标没有回退
     *
     */
    // 获取当前光标
    QTextCursor cursor = ui -> textEdit -> textCursor();
    QString sizeStr = QString::number(size);
    // 获取标题文本
    QString titleText = cursor.block().text();
    // 获取光标所在段落的起始位置和结束位置
    int startPos = cursor.block().position();
    int endPos = startPos + cursor.block().length();

    cursor.setPosition(startPos);

    // 选中段落的文本
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endPos - startPos);

    // 删除选中的文本
    cursor.removeSelectedText();

    // 构建带有 <h1> 标签的文本
    QString htmlText = "<h" + sizeStr + ">" + titleText + "</h" + sizeStr + "><br>";

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

void MainWindow::on_actH1_triggered() {

    // static int size=1;
    // actH(size++);
    // qDebug()<<QString::number(size-1);
    actH(1);
}

void MainWindow::on_actH2_triggered() {
    actH(2);
}

void MainWindow::on_actH3_triggered() {
    actH(3);
}

void MainWindow::on_actH4_triggered() {
    actH(4);
}

void MainWindow::on_actH5_triggered() {
    actH(5);
}

void MainWindow::on_actH6_triggered() {
    /*
     * 问题：当选中多个段落的时候只会把第一个设为h1
     * 光标没有回退
     *
     */
    // 获取当前光标
    QTextCursor cursor = ui -> textEdit -> textCursor();
    // 获取标题文本
    QString titleText = cursor.block().text();
    // 获取光标所在段落的起始位置和结束位置
    int startPos = cursor.block().position();
    int endPos = startPos + cursor.block().length();

    cursor.setPosition(startPos);

    // 选中段落的文本
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endPos - startPos);

    // 删除选中的文本
    cursor.removeSelectedText();

    // 构建带有 <h1> 标签的文本
    QString htmlText = "<p>" + titleText + "</p><br>";

    // 在光标位置插入标题文本（带有 HTML 标签）
    cursor.insertHtml(htmlText);
}

void MainWindow::on_actAddImg_triggered() {
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
    QTextCursor cursor = ui -> textEdit -> textCursor();
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

void MainWindow::on_actOutput_triggered() {

    // 设置富文本内容到 QTextEdit

    // 获取富文本内容的 HTML 表示
    QString html = ui -> textEdit -> toHtml();

    // 创建文件并写入富文本内容
    QString fileName = "output.ext";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream( & file);
        stream << html;
        file.close();
    }
}

void MainWindow::on_actInput_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                                    ".", tr("Bitmap Files (*.ext)\n"));

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream( & file);
        QString html = stream.readAll();
        file.close();

        // 将读取的富文本内容设置到 QTextEdit
        ui -> textEdit -> setHtml(html);
    }
}

void MainWindow::on_FontComboChanged(const QFont & f) {
    if (dontChangeFont == false) {
        // 获取当前 QTextEdit 的文本光标
        QTextCursor cursor = ui -> textEdit -> textCursor();
        QTextCharFormat fmt;
        fmt.setFont(f);
        cursor.mergeCharFormat(fmt);
    } else {
        dontChangeFont = false;
    }

}

void MainWindow::do_changeComboFont() {
    //解决改变combBox又触发改变字体的情况
    dontChangeFont = true;
    // 获取当前 QTextEdit 的文本光标
    QTextCursor cursor = ui -> textEdit -> textCursor();

    // 获取光标所在位置的文本属性
    QTextCharFormat format = cursor.charFormat();

    // 获取字体
    QFont font = format.font();

    fontComboBox -> setCurrentFont(font);
}

void MainWindow::on_actTable_triggered() {
    QTextTableFormat tableFormat;
    tableFormat.setBorderCollapse(true);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    auto * table = ui -> textEdit -> textCursor().insertTable(2, 1, tableFormat);
}

void MainWindow::on_actLast_triggered() {
    if (lastTreeNode.isValid()) {
        nextTreeNode = ui -> treeView -> currentIndex();
        ui -> treeView -> setCurrentIndex(lastTreeNode);
    }
}

void MainWindow::on_actNext_triggered() {
    if (nextTreeNode.isValid()) {
        lastTreeNode = ui -> treeView -> currentIndex();
        ui -> treeView -> setCurrentIndex(nextTreeNode);
    }
}

void MainWindow::restoreTreeViewFromJson(const QJsonObject & jsonObject, QStandardItem * parentItem) {
    QJsonValue idValue = jsonObject["id"]; // 假设从 JSON 对象中获取了 ID 值
    QString sid = idValue.toString();
    uint64_t id = sid.toULongLong();
    // uint64_t id = static_cast<uint64_t>(idValue.toDouble());
    qDebug() << "JSON的id" << id;
    if (idValue.isNull() || idValue.isUndefined() || id == 0 || sid.isEmpty()) {
        //最外层的情景
        if (jsonObject.contains("children")) {
            QJsonArray childrenArray = jsonObject["children"].toArray();

            // 递归恢复子节点
            for (const QJsonValue & jsonValue: childrenArray) {
                QJsonObject childObject = jsonValue.toObject();
                restoreTreeViewFromJson(childObject, parentItem);
            }
        }

    } else {
        QString currentText = jsonObject["name"].toString();
        // 创建当前节点
        QStandardItem * item = new QStandardItem(currentText);

        QVariant idVariant = QVariant::fromValue(id);
        //uint64_t id = idVariant.value<uint64_t>();

        item -> setData(idVariant, Qt::UserRole);

        //从setdata中取出data的代码
        QVariant idVariant2 = item -> data(Qt::UserRole);
        uint64_t id2 = idVariant.value < uint64_t > ();
        qDebug() << "刚取出来的" << id2;

        // 添加当前节点到父节点或根节点
        if (parentItem) {
            parentItem -> appendRow(item);
        } else
            m_standardItemModel -> appendRow(item);

        // 检查是否有子节点
        if (jsonObject.contains("children")) {
            QJsonArray childrenArray = jsonObject["children"].toArray();

            // 递归恢复子节点
            for (const QJsonValue & jsonValue: childrenArray) {
                QJsonObject childObject = jsonValue.toObject();
                restoreTreeViewFromJson(childObject, item);
            }
        }
    }

}

void MainWindow::on_actClip_triggered() {
    auto textEdit = ui -> textEdit;
    QString selectedText = textEdit -> textCursor().selectedText();
    QString replaceText;
    if (selectedText.length() <= 2) {
        replaceText = "[问题]";
    } else {
        int count = (selectedText.length() - 2) / 2;
        replaceText = "[";
        for (int i = 0; i < count; i++) replaceText += "-";
        replaceText += "问题";
        for (int i = 0; i < count; i++) replaceText += "-";
        replaceText += "]";
    }

    QTextCharFormat format = textEdit -> textCursor().charFormat();
    format.setForeground(Qt::red);

    textEdit -> textCursor().beginEditBlock();

    textEdit -> textCursor().removeSelectedText();
    textEdit -> textCursor().insertText(replaceText, format);

    textEdit -> textCursor().endEditBlock();
}

void MainWindow::on_actFind_triggered() {
    auto textEdit = ui -> textEdit;
    QString html = textEdit -> toHtml();

}

void MainWindow::restoreTree() {
    //m_standardItemModel->clear();
    if (m_standardItemModel) {
        m_standardItemModel -> clear(); // 清空模型数据
        // 使用model设置QTreeView表头
        m_standardItemModel -> setHorizontalHeaderLabels(QStringList(QStringLiteral("名称")));
    }

    QFile file("tree_data.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray jsonData = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = jsonDoc.object();

        // 调用 restoreTreeViewFromJson 函数
        restoreTreeViewFromJson(jsonObject, m_standardItemModel -> invisibleRootItem());

        file.close();
    }

}

void MainWindow::saveTree() {
    QStandardItem * rootItem = m_standardItemModel -> invisibleRootItem(); // 获取根节点
    QJsonObject jsonObject; // 创建空的 JSON 对象
    // 调用 saveTreeViewToJson 函数
    saveTreeViewToJson(rootItem, jsonObject);

    // 将 JSON 数据写入文件
    QJsonDocument jsonDoc(jsonObject);
    QFile file("tree_data.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
    }

}

void MainWindow::setTreeTestData() {
    // 创建根节点，抽象Item，并没有实际数据
    QStandardItem * itemRoot = m_standardItemModel -> invisibleRootItem();

    uint64_t id = 123456789; // 假设要保存的 ID

    QVariant idVariant;
    idVariant.setValue(id);

    // 创建并添加Item的第一个子节点
    QStandardItem * itemCam = new QStandardItem(QStringLiteral("相机"));
    idVariant.setValue(generator -> generateGlobalId());
    itemCam -> setData(idVariant, Qt::UserRole);
    itemRoot -> appendRow(itemCam);

    // 创建并添加Item的第二个子节点
    QStandardItem * itemImg = new QStandardItem(QStringLiteral("图片"));
    itemRoot -> appendRow(itemImg);
    idVariant.setValue(generator -> generateGlobalId());
    itemImg -> setData(idVariant, Qt::UserRole);

    QStandardItem * itemTest = new QStandardItem(QStringLiteral("三级"));
    itemRoot -> appendRow(itemTest);
    idVariant.setValue(generator -> generateGlobalId());
    itemTest -> setData(idVariant, Qt::UserRole);

    QStandardItem * itemTest4 = new QStandardItem(QStringLiteral("四"));
    idVariant.setValue(generator -> generateGlobalId());
    itemTest4 -> setData(idVariant, Qt::UserRole);

    itemTest -> appendRow(itemTest4);
    QStandardItem * itemTest5 = new QStandardItem(QStringLiteral("五"));
    idVariant.setValue(generator -> generateGlobalId());
    itemTest5 -> setData(idVariant, Qt::UserRole);

    itemTest4 -> appendRow(itemTest5);
}

void MainWindow::do_treeSelectChanged(const QModelIndex & current,
                                      const QModelIndex & previous) {
    lastTreeNode = previous;
    QString html;
    QStandardItem * item = m_standardItemModel -> itemFromIndex(current);
    if (item) {
        //你的操作,同上
        //QMessageBox::information(this,"msg","slotCurrentChanged: " + item->text());
        QVariant idVariant = item -> data(Qt::UserRole);
        uint64_t id = idVariant.value < uint64_t > ();
        sqlUtil -> getArticle(id, html);

        qDebug() << "刚取出来的id(int64)" << id;
        qDebug() << "刚取出来的html" << html;
        //derorateText(result);
        ui -> textEdit -> setHtml(html);

    }

}

void MainWindow::do_saveNote() {
    QString html = ui -> textEdit -> toHtml();
    // qDebug()<<"planText取出的html"<<html;
    QStandardItem * item = m_standardItemModel -> itemFromIndex(ui -> treeView -> currentIndex());
    if (item) {
        //你的操作,同上
        //QMessageBox::information(this,"msg","slotCurrentChanged: " + item->text());
        QVariant idVariant = item -> data(Qt::UserRole);
        uint64_t id = idVariant.value < uint64_t > ();

        sqlUtil -> setArticle(id, html);

        // qDebug()<<"刚取出来的id(int64)"<<id;
        // qDebug()<<"刚取出来的html"<<html;

    }
}

void MainWindow::derorateText(QString & s) {
    QString result = "";
    int i = 0;
    while ((i + 1) < s.length()) {

        if (s[i] == '\\' && s[i + 1] == '"') {
            i++;
            continue;
        }
        result += s[i];
        i++;
    }
}

void MainWindow::on_actNewNote_triggered() {
    auto treeView = ui -> treeView;
    QModelIndex currentIndex;
    QAbstractItemModel * model = treeView -> model();

    QModelIndexList selectedIndexes = ui -> treeView -> selectionModel() -> selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        currentIndex = selectedIndexes.first();
    } else {
        currentIndex = model -> index(0, 0); // 获取第一行第一列的索引
    }

    int row = currentIndex.row(); // 获取当前节点的行号
    QModelIndex parentIndex = currentIndex.parent(); // 获取当前节点的父节点索引

    // 在同级中插入新元素
    bool success = model -> insertRow(row + 1, parentIndex);
    if (success) {

        QModelIndex newIndex = model -> index(row + 1, currentIndex.column(), parentIndex);
        treeView -> selectionModel() -> setCurrentIndex(newIndex, QItemSelectionModel::Select);
        uint64_t uuid = generator -> generateGlobalId();
        sqlUtil -> insertNote(uuid, "新笔记", 1, "", "");

        QVariant idVariant = QVariant::fromValue(uuid);
        ui -> treeView -> model() -> setData(newIndex, idVariant, Qt::UserRole);

        // QVariant idVariant = item->data(Qt::UserRole);
        // uint64_t id = idVariant.value<uint64_t>();

        // 使新插入的节点处于编辑状态
        treeView -> edit(newIndex);
        //检测是否输入了名字
        qDebug() << "bmjijxsuu";
    }
}

void MainWindow::on_actDel_triggered() {
    QAbstractItemModel * model = ui -> treeView -> model();
    QModelIndex currentIndex = ui -> treeView -> currentIndex();
    int row = currentIndex.row();
    model -> removeRow(currentIndex.row(), currentIndex.parent());
    QModelIndex newIndex = model -> index(row, currentIndex.column(), currentIndex.parent());
    selectionModel -> setCurrentIndex(newIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::on_actLearn_triggered()
{
    if(!learnWindow){
        learnWindow=new LearnWindow;
    }
    //传输model给learnWid;
    connect(this,&MainWindow::transferTreeModel,learnWindow,&LearnWindow::receiveTreeModel);
    emit transferTreeModel(this->m_standardItemModel);

    //同步本窗口和另一个窗口的选择
    connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::handleSelectionChanged);
    connect(learnWindow->getTreeviewPointer()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::handleSelectionChanged);

    learnWindow->show();
}

void MainWindow::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    // 将 treeView1 的选择状态应用到 treeView2
    ui->treeView->selectionModel()->select(selected, QItemSelectionModel::Select);
    ui->treeView->selectionModel()->select(deselected, QItemSelectionModel::Deselect);

}

void MainWindow::OnRightClicked(const QPoint &point)
{
    QTreeView* curTree = qobject_cast<QTreeView*>(sender());
    QModelIndex index = curTree->indexAt(point);  // point 是右键点击的坐标

    if (index.isValid()) {
        // 获取当前右键点击的项
        QStandardItem* selectedItem = static_cast<QStandardItem*>(index.internalPointer());
        if (selectedItem) {
            // 对选中项进行处理
            // 例如，获取选中项的数据：
            QVariant data = selectedItem->data(Qt::DisplayRole);
            // 其他处理...
            treeMenu->exec(QCursor::pos());

        }
    }





}



void MainWindow::setSettings(QSettings *newSettings)
{
    settings = newSettings;
}

void MainWindow::initTreeMenu()
{
    treeMenu = new QMenu(this);
    treeMenu->addAction(new QAction("MyAction0001"));
    treeMenu->addAction(new QAction("MyAction0004"));
    treeMenu->addAction(new QAction("MyAction0006"));


}

void MainWindow::setDebugOutput(const QString &rawTargetFilePath_, const bool &argDateFlag_)
{

    static QString rawTargetFilePath;
    static bool argDateFlag;

    rawTargetFilePath = rawTargetFilePath_;
    argDateFlag = argDateFlag_;

    class HelperClass
    {
    public:
        static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message_)
        {
            QString message;

            switch ( type )
            {
            case QtDebugMsg:
            {
                message = message_;
                break;
            }
            case QtWarningMsg:
            {
                message.append("Warning: ");
                message.append(message_);
                break;
            }
            case QtCriticalMsg:
            {
                message.append("Critical: ");
                message.append(message_);
                break;
            }
            case QtFatalMsg:
            {
                message.append("Fatal: ");
                message.append(message_);
                break;
            }
            default: { break; }
            }

            QString currentTargetFilePath;

            if ( argDateFlag )
            {
                currentTargetFilePath = rawTargetFilePath.arg( ( ( argDateFlag ) ? ( QDateTime::currentDateTime().toString("yyyy_MM_dd") ) : ( "" ) ) );
            }
            else
            {
                currentTargetFilePath = rawTargetFilePath;
            }

            if ( !QFileInfo::exists( currentTargetFilePath ) )
            {
                QDir().mkpath( QFileInfo( currentTargetFilePath ).path() );
            }

            QFile file( currentTargetFilePath );
            file.open( QIODevice::WriteOnly | QIODevice::Append );

            QTextStream textStream( &file );
            textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" ) << ": " << message << Qt::endl;
        }
    };

    qInstallMessageHandler( HelperClass::messageHandler );
}

