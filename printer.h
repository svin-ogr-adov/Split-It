#ifndef PRINTER_H
#define PRINTER_H

//#include<ui_mainwindow.h>
#include<split_info.h>
#include<reader.h>

using str = std::string;
using v_str = std::vector<std::string>;
using v_v_int = std::vector<std::vector<int>>;
using file_data = std::vector<std::vector<std::string>>;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class printer : public QMainWindow
{
    Ui::MainWindow *_morda;
    reader *_reader;
public:
    printer(Ui::MainWindow *ui);
    ~printer();

    str
    createTitleRow
    (const str& first_row, const v_v_int& mask);

    bool
    compareRows
    (const v_str& lhs_row, const v_str& rhs_row);

    std::string
    rowCellsToString
    (v_str& row_cells);

    v_v_int
    createMask
    (const str& _str);

    str
    modifyStringByTemplate
    (v_v_int& mask, v_str& row_cells);

    void
    printRange
    (file_data& f_data,
           str& destination,
           bool add_first_row,
     const str& edited_first_row,
            int files_quantity,
            int rows_quantity,
            int file_start_position,
            int row_start_position);

    void
    printFiles
    (file_data& f_data,
           str& destination,
    split_info& _split_info,
           bool add_row,
     const str& edited_first_row);
};

#endif // PRINTER_H
