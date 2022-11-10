#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file_info.h"
#include "calculator.h"
#include <iostream>
#include <QRegularExpression>
#include <reader.h>
#include <printer.h>
#include<QDesktopServices>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   // setWindowIcon(QIcon(":123.svg"));
    ui->setupUi(this);
    centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);
    statusBar()->hide();
    menuWidget()->hide();
    ui->splitButton->setDisabled(true);
    ui->lineEditSource->setPlaceholderText("Select source file: *.csv");
    ui->lineEditDestination->setText(QDir::homePath()+"/Downloads");
//    ui->pushButtonReloadSourceFile->setDisabled(true);
//    ui->pushButtonShowPreview->setDisabled(true);

    QRegularExpression rowRange("([1-9]|[1-9][0-9]{1,5})");
    rowRangeValidator = new QRegularExpressionValidator(this);
    rowRangeValidator->setRegularExpression(rowRange);
    ui->lineEditRowsQuantity->setValidator(rowRangeValidator);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete rowRangeValidator;
}

void MainWindow::updateParseInfo( QString& path) {
    auto f_info = getFileInfo(path);
    reader _reader(ui);
    if (f_info.first != "error") {
        if (f_info.first == "csv" && f_info.second == "text/plain") {
            result = _reader.CSVparser(path, 0);
            TotalRows = result.size();
            ui->splitButton->setEnabled(true);
        } else if (f_info.first == "xlsx" && f_info.second == "application/zip") {
            TotalRows = result.size();
            ui->splitButton->setEnabled(true);
        } else if (f_info.first == "csv" && f_info.second == "application/zip") {
            ui->textBrowser->setText("File extention *csv, but it's *xlsx indeed");
            TotalRows = result.size();
            ui->splitButton->setEnabled(true);
        } else if (f_info.first == "xlsx" && f_info.second == "text/plain") {
            ui->textBrowser->setText("File extention *xlsx, but it's *csv indeed");
            //result = _parser->CSVparser(path, ui);
            TotalRows = result.size();
            ui->splitButton->setEnabled(true);
        } else if((f_info.first == "csv" || f_info.first == "xlsx") &&
                  (f_info.second != "text/plain" && f_info.second == "application/zip"))
        {
            ui->textBrowser->setText("File extention: "+f_info.first+", but it's other file indeed");
            ui->textBrowser->append("Looks like: "+f_info.second);
            ui->textBrowser->append("Nothing to do");
            ui->splitButton->setDisabled(true);
        }
    } else {
        ui->textBrowser->setText(f_info.second);
    }
}

void MainWindow::updateQantityInfo()
{
    bool is_strict = ui->checkBoxStrictSplit->isChecked();
    bool add_first_row = ui->checkBoxAddFirstRow->isChecked();
    size_t rows_in_file = ui->lineEditRowsQuantity->text().toInt();

    _split_info = new split_info(calculator(TotalRows, rows_in_file, is_strict, add_first_row));

    ui->textBrowser->setText("Total rows in source file: "+QString::number(TotalRows)+'\n');
    if (_split_info->first_files) {
        ui->textBrowser->append("To export:\n"+QString::number(_split_info->first_files)+
                                  +" "+(_split_info->first_files==1 ? "file":"files")+" with "
                                  +QString::number(_split_info->first_rows)+" rows");
    }
    if (_split_info->last_files) {
        ui->textBrowser->append(QString::number(_split_info->last_files)
                                  +" "+(_split_info->last_files==1 ? "file":"files")
                                  +" with "+QString::number(_split_info->last_rows)+" rows");
    }
}

std::string MainWindow::getFirstRowFromTableWidget() {
    std::string result;
    int cells = ui->tableWidgetMain->model()->columnCount();
    for (int i = 0; i < cells; ++i) {
        result.append(ui->tableWidgetMain->item(0,i)->text().toStdString());
        result.append(",");
    }
    result.pop_back();
    return result;
}

std::string MainWindow::readFirstRow() {
    std::string result;
    int cells = ui->tableWidgetMain->model()->columnCount();
    for (int i = 0; i < cells; ++i) {
        auto r = ui->tableWidgetMain->item(0,i)->text().toStdString();
        if (r != "") {
            result.append(r+',');
        }
    }
    result.pop_back();
    return result;
}

void MainWindow::fillTableWidget( file_data& f_data)
{
    for(size_t row_num = 0; row_num < f_data.size(); row_num++) {
        for(size_t cell_num = 0; cell_num < f_data[row_num].size(); cell_num++) {
            ui->tableWidgetMain->setItem(row_num, cell_num,
            new QTableWidgetItem(QString::fromStdString(f_data[row_num][cell_num])));
        }
    }
}
void MainWindow::fillTableWidgetPreview( file_data& f_data)
{
    for(size_t row_num = 0; row_num < f_data.size(); row_num++) {
        for(size_t cell_num = 0; cell_num < f_data[row_num].size(); cell_num++) {
            ui->tableWidgetPreview->setItem(row_num, cell_num,
            new QTableWidgetItem(QString::fromStdString(f_data[row_num][cell_num])));
        }
    }
}
char MainWindow::getCellDelimSetting(){
    return ui->comboBoxCell->currentText().toStdString()[0];
}
char MainWindow::getTextDelimSetting(){
    return ui->comboBoxText->currentText().toStdString()[0];
}
size_t MainWindow::getRowsQuantitySetting(){
    return ui->lineEditRowsQuantity->text().toInt();
}
std::string MainWindow::getSourcePathSetting(){
    return ui->lineEditSource->text().toStdString();
}
std::string MainWindow::getDestinationSetting(){
    return ui->lineEditDestination->text().toStdString();
}

std::string MainWindow::createExtractionFolder(const QString& path) {
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy_hh-mm-ss");
    QString _path = path+"/"+"Extraction_"+formattedTime+"/";
    QDir dir(_path);
    if (!dir.exists()){
      dir.mkpath(".");
    }
    return _path.toStdString();
}

void MainWindow::on_splitButton_clicked()
{
    printer _p(ui);
    std::string path = createExtractionFolder(ui->lineEditDestination->text());
    _p.printFiles
    (
        result,
        path,
        *_split_info,
        ui->checkBoxAddFirstRow,
        getFirstRowFromTableWidget()
    );
    QDesktopServices::openUrl(QString::fromStdString(path));
}

void MainWindow::on_checkBoxStrictSplit_stateChanged(int arg1)
{
    if (arg1 >= 0 && currentFilePath !="") {updateQantityInfo();}
}


void MainWindow::on_checkBoxAddFirstRow_stateChanged(int arg1)
{
    if (arg1 >= 0 && currentFilePath !="") {updateQantityInfo();}
}

void MainWindow::on_lineEditRowsQuantity_textChanged(const QString &arg1)
{
    if (arg1.toInt() >= 0 && currentFilePath !="") {updateQantityInfo();}
}

void MainWindow::on_destinationButton_clicked()
{
    auto path = destinationPath->getExistingDirectory(this,
                                                      tr("Select destination folder"),
                                                      ui->lineEditDestination->text(),
                                                      QFileDialog::ShowDirsOnly);
    if (!path.isEmpty()) ui->lineEditDestination->setText(path);
}

void MainWindow::loadFile() {
    if(!currentFilePath.isEmpty()&& !currentFilePath.isNull()){
        updateParseInfo(currentFilePath);
        updateQantityInfo();
        ui->tableWidgetMain->showGrid();
        ui->tableWidgetMain->autoFillBackground();
        ui->tableWidgetMain->setRowCount(TotalRows);
        ui->tableWidgetMain->setColumnCount(result[0].size());
        fillTableWidget(result);
    }
}

void MainWindow::loadPreview(int rows_quantity) {
    if(!currentFilePath.isEmpty()&& !currentFilePath.isNull()){
//updateParseInfo(currentFilePath);
  //      updateQantityInfo();
        reader _reader(ui);
        QString g = ui->lineEditSource->text();
        result = _reader.CSVparser(g, rows_quantity);
        ui->tableWidgetPreview->showGrid();
        ui->tableWidgetPreview->autoFillBackground();
        ui->tableWidgetPreview->setColumnCount(result[0].size());
        if (TotalRows > rows_quantity) {
            ui->tableWidgetPreview->setRowCount(rows_quantity);
            //file_data _result(result.begin(), result.begin()+rows_quantity);
            fillTableWidgetPreview(result);
        } else {
            ui->tableWidgetPreview->setRowCount(TotalRows);
            fillTableWidgetPreview(result);
        }
    }
}

void MainWindow::on_sourceButton_clicked()
{
    currentFilePath = sourcePath->getOpenFileName(this
                                                ,QDir::homePath()+"/Downloads"
                                                ,tr("*.csv"));

    if (!currentFilePath.isEmpty()) ui->lineEditSource->setText(currentFilePath);
    //ui->splitButton->setText("Stop");
    loadFile();
}

void MainWindow::on_pushButtonReloadSourceFile_clicked()
{
    loadFile();
}

void MainWindow::on_pushButtonShowPreview_clicked()
{
    loadPreview(13);
}

void MainWindow::on_comboBoxCell_currentTextChanged(const QString &arg1)
{
    cell_delim = arg1.toStdString()[0];
}

void MainWindow::on_comboBoxText_currentTextChanged(const QString &arg1)
{
    text_delim = arg1.toStdString()[0];
}

void MainWindow::on_lineEditSource_textEdited(const QString &arg1)
{
    if (arg1 == "") {
        ui->lineEditSource->setPlaceholderText("Select source file: *.csv");
        ui->pushButtonReloadSourceFile->setDisabled(true);
        ui->pushButtonShowPreview->setDisabled(true);

    } else {
        ui->pushButtonReloadSourceFile->setEnabled(true);
        ui->pushButtonShowPreview->setEnabled(true);
    }
}

void MainWindow::on_lineEditDestination_textEdited(const QString &arg1)
{
    if (arg1 == "") {
        ui->lineEditDestination->setText(QDir::homePath()+"/Downloads");
    }
}
void MainWindow::on_lineEditSource_selectionChanged()
{
    qDebug() << "!";
}

void MainWindow::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("https://docs.google.com/document/d/1SGVSvWFZZ0zS8duFyI_Mw8QKIsUNmNJE3aJ0eIlo2Og/edit?usp=sharing"));
}
