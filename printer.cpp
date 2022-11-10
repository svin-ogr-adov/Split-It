#include "printer.h"
#include<fstream>
#include <iostream>
#include "trim.h"


printer::printer(Ui::MainWindow *ui)
: _morda(ui)
{
    _reader = new reader(_morda);
}

printer::~printer(){
    delete _reader;
}

bool printer::compareRows(const v_str& lhs_row,
                          const v_str& rhs_row)
{
    if (lhs_row.size() != rhs_row.size()) return false;
    for (size_t i = 0; i < lhs_row.size(); i++) {
        if (lhs_row[i] != rhs_row[i]) return false;
    }
    return true;
}

str printer::rowCellsToString(v_str& row_cells) {
    str result;
    for ( auto& cell : row_cells) {
        result+=cell+',';
    }
    result.pop_back();
    return result;
}

str printer::createTitleRow(const str& first_row,
                      const v_v_int& mask) {
    auto row_cells = _reader->splitRowByCells(first_row);
    str result;
    for (auto i : mask){
        result+=row_cells.at(i.at(0))+",";
    }
    result.pop_back();
    return result;
}

v_v_int printer::createMask(const str& _str) {
    auto mask_cells =  _reader->splitRowByCells(_str);
    std::multimap<str, size_t> cell_to_position; {}
    for (size_t i = 0; i < mask_cells.size(); i++) {
        mask_cells[i] = trim(mask_cells[i]);
        if (mask_cells[i] != "") {
            cell_to_position.insert({mask_cells[i], i});
        }
    }
    v_v_int mask;
    for (auto it = cell_to_position.begin();
              it != cell_to_position.end();) {
        std::vector<int> temp;
        auto p = cell_to_position.equal_range(it->first);
        while (p.first != p.second)
        {
            temp.push_back(p.first++->second);
        }
        mask.push_back(temp);
        it = p.second;
    }
    std::sort(mask.begin(), mask.end());
    return mask;
}

str printer::modifyStringByTemplate(v_v_int& mask, v_str& row_cells) {
    str  result;
    try {
        for ( std::vector<int>& cell_positions : mask) {
            for (int cell_num : cell_positions) {
                result += row_cells[cell_num]+"|";
            }
            result.pop_back();
            result+=",";
        }
        result.pop_back();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return result;
}

void printer::printRange
(
    file_data& f_data,
          str& destination,
          bool add_first_row,
    const str& edited_first_row,
           int files_quantity,
           int rows_quantity,
           int file_start_position,
           int row_start_position
)
{
    auto mask = createMask(edited_first_row);
    auto editedTitleRow = createTitleRow(edited_first_row, mask);
    bool need_modify_row = !compareRows(f_data[0], _reader->splitRowByCells(edited_first_row));
    int row_idx = row_start_position;
    std::ofstream fstream;
    while (file_start_position <= files_quantity)
    {
        int q_idx = rows_quantity;
        fstream.open(destination+"extraction_"+std::to_string(file_start_position)+".csv");
        bool first_row_added = false;
        while (q_idx)
        {
            if (!first_row_added && add_first_row) {
                if (row_idx == 0) {
                    fstream << editedTitleRow <<'\n';
                    ++row_idx;
                } else {
                    fstream << editedTitleRow <<'\n';
                }
                first_row_added = true;
            } else {
                fstream << (need_modify_row ? modifyStringByTemplate(mask, f_data[row_idx])
                                            : rowCellsToString(f_data[row_idx])) << '\n';
                ++row_idx;
            }
            --q_idx;
        }
        fstream.flush();
        fstream.close();
        ++file_start_position;
    }
}

void printer::printFiles
(
     file_data& f_data,
           str& destination,
    split_info& _split_info,
           bool add_row,
     const str& edited_first_row
)
{
    int first_file_start_position = 1;
    int first_row_start_position = 0;
    int last_row_start_position = add_row ? _split_info.first_files*(_split_info.first_rows-1) + 1
                                          : _split_info.first_files*(_split_info.first_rows-1);
    int last_file_start_position = _split_info.first_files + 1;


    if (_split_info.first_files) {
        printRange(f_data,
                   destination,
                   add_row,
                   edited_first_row,
                   _split_info.first_files,
                   _split_info.first_rows,
                   first_file_start_position,
                   first_row_start_position);
    }


    if (_split_info.last_files) {
        printRange(f_data,
                   destination,
                   add_row,
                   edited_first_row,
                   _split_info.total_files,
                   _split_info.last_rows,
                   last_file_start_position,
                   last_row_start_position);
    }
}
