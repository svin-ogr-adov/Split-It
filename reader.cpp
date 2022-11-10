#include "reader.h"
#include "ui_mainwindow.h"
#include<fstream>
#include <future>
//#include"csv.h"
#include<QString>
#include<iostream>
#include <QApplication>
#include <QDebug>

reader::reader(Ui::MainWindow *morda)
    : _morda(morda)
{
                 cell_d = _morda->comboBoxCell->currentText().toStdString()[0];
                  txt_d = _morda->comboBoxText->currentText().toStdString()[0];
         isMultipleRows = _morda->checkBoxMultipleRows->isChecked();
removeRowsWithoutEmails = _morda->checkBoxRemoveRowsWithoutEmails->isChecked();
}
reader::reader()
{

}
reader::~reader()
{

}

str reader::fixDelimeter(str& _str) {

    for(size_t i = 0; i < _str.size(); ++i)
    {
        if(_str[i] == ',' ||
           _str[i] == ';' ||
           _str[i] == '*' ||
           _str[i] == ':')
        {
            _str[i] = '|';
        }
    }
    return "|";
}

v_str reader::splitString(str& _str, str& delim) {
    v_str result;
    size_t pos = 0;
    str token;
    while ((pos = _str.find(delim)) != str::npos) {
        token = _str.substr(0, pos);
        result.push_back(token);
        _str.erase(0, pos + delim.length());
    }
    result.push_back(_str);
    return result;
}

v_str reader::splitRowByCells(const str& _str)
{
    v_str result;
    if (_str.size()==0){
        result.push_back("");
        return result;
    }
    bool br = false;
    str temp_cell = "";
    for (size_t i = 0; i < _str.size(); ++i)
    {
        char ch = _str[i];
        char ch_next = _str[i+1];

        if (ch == txt_d) br ? br = false : br = true;

        if (ch != cell_d) temp_cell.push_back(ch);

        if (ch == cell_d && ch_next != cell_d) {
            if (br) {
                temp_cell.push_back(ch);
            } else {
                result.push_back(temp_cell);
                temp_cell = "";
            }
        }
        if (ch == cell_d && ch_next == cell_d) {
            if (br) {
                temp_cell.push_back(ch);
            } else {
                result.push_back(temp_cell);
                temp_cell = "";
            }
        }
        if (i == _str.size()-1 && !br) {
            result.push_back(temp_cell);
        } else if(i == _str.size()-1 && br){
            result.push_back("");
        }
    }
    return result;
}

std::pair<bool,size_t> reader::getCellIdx(const v_str& cells,
                                          const str& cell_name) {
    for (size_t i = 0; i < cells.size(); ++i) {
        if (cells[i] == cell_name) {
            return {true, i};
        }
    }
    return {false, 0};
}

bool reader::isSymbolValid(const char& i) {
    return (i >= '0' && i <= '9') ||
           (i >= 'a' && i <= 'z') ||
           (i >= 'A' && i <= 'Z');
}

void reader::trimStr(str& _str) {
    size_t s = _str.size();
    if (s == 0) return;
    if (s == 1 && !isSymbolValid(_str.at(0))) {
        _str = "";
        return;
    }
    size_t i = 0;
    size_t i2 = s-1;
    if (s > 1) {
        for (;i < s; ++i) {
            if (isSymbolValid(_str.at(i))) {
                break;
            }
        }
        for (;i2 !=0 ; --i2) {
            if (isSymbolValid(_str.at(i2))) {
                break;
            }
        }
        std::string d;
        for (;i <= i2; ++i) {
            d.push_back(_str.at(i));
        }
        _str = d;
    }
}

file_data reader::sortOutEmailCell(int mail_idx, v_str& cells) {
    str delim = fixDelimeter(cells[mail_idx]);
    v_str emails;
    if (delim != "") {
        emails = splitString(cells[mail_idx], delim);
    }
    file_data result;
    if (emails.size()) {
        for (str& email : emails) {
            trimStr(email);
            cells[mail_idx] = email;
            result.push_back(cells);
        }
    }
    return result;
}

void reader::stringFilter(str& _str) {
    std::string result;
    for (auto it = _str.begin(); it != _str.end(); ++it) {
        if ((*it >= '0' && *it <= '9') ||
            (*it >= 'a' && *it <= 'z') ||
            (*it >= 'A' && *it <= 'Z') ||
             *it == '_' || *it == ',' ||
             *it == ';' || *it == '|' ||
             *it == '.' || *it == '/' ||
             *it == '@' || *it == ' ' ||
             *it == '-' || *it == '?' ||
             *it == '!'  )
        {
                result += *it;
        }
    }
    _str = result;
}

bool reader::setText(int i) {
    _morda->textBrowser->setText("Rows readed: "+QString::number(i));
    return true;
}

file_data reader::CSVparser(QString& file_path, int iter)
{

    int it = iter;
    file_data _file_data;
    v_str row_cells;
    v_str f_row_cells;
    str _str;
    std::ifstream file(file_path.toStdString(), std::ios::binary);

//    while (getline(file, _str)) {
//        row_cells = splitRowByCells(_str);
//        _file_data.push_back(row_cells);
//    }
//    return _file_data;



    v_str first_row_cells;
    bool is_first_row = true;
    while (getline(file, _str)) {
        _str.erase(std::remove(_str.begin(), _str.end(), '\r'), _str.end());
        f_row_cells = splitRowByCells(_str);
        break;
    }

    auto email_idx = getCellIdx(f_row_cells, "email");

    std::pair<bool,size_t> phones_idx;
    if (getCellIdx(f_row_cells, "phones").first) {
        phones_idx = {true,1};
    }
    if (getCellIdx(f_row_cells, "phone").first) {
        phones_idx = {true,1};
    }

//    auto is_vip_idx = getCellIdx(f_row_cells, "is_vip");
    _file_data.push_back(f_row_cells);
   // int g = 0;
    while (getline(file, _str)) {
        // FOR PREVIEW INTERRUPT
        if(it) {
            --it;
            if (!it) return _file_data;
        }
 //       qApp->processEvents();
   //     setText(++g);
        _str.erase(std::remove(_str.begin(), _str.end(), '\r'), _str.end());
        //stringFilter(_str); // added here
        row_cells = splitRowByCells(_str);
        if (is_first_row) {
            first_row_cells = row_cells;
            is_first_row = false;
        }
        // adding cells (if any row has less cells than first row)
        int addition = first_row_cells.size() - row_cells.size();
        if (addition > 0) {
            row_cells.insert(row_cells.end(), addition, ",");
        }
//        for(std::string& cell : row_cells) {
//            trimStr(cell);
//            stringFilter(cell);
//        }
//        if (is_vip_idx.first) {
//            if (row_cells[is_vip_idx.second] == "Yes") {
//                row_cells[is_vip_idx.second] = "1";
//            }
//            if (row_cells[is_vip_idx.second] == "No") {
//                row_cells[is_vip_idx.second] = "0";
//            }
//        }
        if (phones_idx.first) {
            fixDelimeter(row_cells[phones_idx.second]);
        }

            if (email_idx.first &&
                row_cells[email_idx.second] !="")
            {
                if(isMultipleRows){
                    for ( auto& i : sortOutEmailCell(email_idx.second, row_cells)) {
                        _file_data.push_back(i);
                    }
                } else {
                    _file_data.push_back(row_cells);
                }
            } else if (!removeRowsWithoutEmails){
                _file_data.push_back(row_cells);
            }
    }
    return _file_data;
}





