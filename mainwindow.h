#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QRegularExpressionValidator>
#include <split_info.h>
#include <QCheckBox>
class reader;
class printer;

using vec_of_str = std::vector<std::string>;
using file_data = std::vector<std::vector<std::string>>;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    char cell_delim;
    char text_delim;
    bool del_row_without_email;
    bool multiple_row_with_more_than_one_email;
    void moveProgressBar(int position);
    void setProgressBarRange(int range);
    char getCellDelimSetting();
    char getTextDelimSetting();
    size_t getRowsQuantitySetting();
    std::string getSourcePathSetting();
    std::string getDestinationSetting();
    Ui::MainWindow *ui;
    //parser *_parser;
    char getCellDelim();
    char getTextDelim();

private slots:

    void on_splitButton_clicked();
    void on_checkBoxStrictSplit_stateChanged(int arg1);
    void on_checkBoxAddFirstRow_stateChanged(int arg1);
    void on_lineEditRowsQuantity_textChanged(const QString &arg1);
    void on_destinationButton_clicked();
    void on_sourceButton_clicked();
    void on_pushButtonReloadSourceFile_clicked();
    void on_pushButtonShowPreview_clicked();
    void on_comboBoxCell_currentTextChanged(const QString &arg1);
    void on_comboBoxText_currentTextChanged(const QString &arg1);
    void on_lineEditSource_textEdited(const QString &arg1);
    void on_lineEditDestination_textEdited(const QString &arg1);
    void on_lineEditSource_selectionChanged();
    void on_pushButton_clicked();


private:



    std::string editedFirstString;
    QFileDialog *sourcePath;
    QFileDialog *destinationPath;
    QStringList file_filters{"*csv","*xlsx"};
    std::vector<std::string> output;
    QString currentFilePath;
    file_data result;
    int TotalRows;
    int TotalFiles;
    int rowsInFile;
    size_t total_files;
    size_t first_files;
    size_t last_files;
    size_t first_rows;
    size_t last_rows;
    split_info *_split_info;

    void loadFile();
    void loadPreview(int rows_quantity);
    std::string createExtractionFolder(const QString& path);
    void fillTableWidget( file_data& rows);
    void updateParseInfo( QString& path);
    void updateQantityInfo();
    void fillTableWidgetPreview( file_data& f_data);
    std::string getFirstRowFromTableWidget();
    std::string readFirstRow();
    QRegularExpressionValidator *rowRangeValidator;
};
#endif // MAINWINDOW_H
