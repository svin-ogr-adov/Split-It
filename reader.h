#ifndef READER_H
#define READER_H

#include<mainwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using str = std::string;
using v_str = std::vector<std::string>;
using v_v_int = std::vector<std::vector<int>>;
using file_data = std::vector<std::vector<std::string>>;

class reader : public QMainWindow
{
public:
    reader(Ui::MainWindow *morda);
    reader();
    ~reader();

    v_str splitRowByCells(const str& _str);
    file_data CSVparser(QString& file_path, int iter = 0);

private:
    Ui::MainWindow *_morda;
    char txt_d;
    char cell_d;
    bool isMultipleRows;
    bool removeRowsWithoutEmails;


    bool
    setText
    (int i);

    str
    fixDelimeter
    (str& _str);

    v_str
    splitString
    (str& _str, str& delim);

    std::pair<bool, size_t>
    getCellIdx
    (const v_str& cells, const str& cell_name);

    bool
    isSymbolValid
    (const char& i);

    void
    trimStr
    (str& _str);

    file_data
    sortOutEmailCell
    (int mail_idx, v_str& cells);

    void
    stringFilter
    (str& _str);
};

#endif // READER_H
