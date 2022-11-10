#include<split_info.h>
#include<calculator.h>


split_info calculator( int rows,  int delimetr, bool is_strict, bool add_first_row) {

    if (rows <= delimetr || !delimetr || delimetr == 1) {
        return
        {
            total_files(1),
            first_files(1),
            last_files(0),
            first_rows(rows),
            last_rows(0)
        };
    }

    int total_rows = add_first_row ? (rows % delimetr ? rows+rows/delimetr : rows+rows/delimetr-1) : rows;
    int _total_files = rows % delimetr ? total_rows/delimetr+1 : total_rows/delimetr;
    int _first_files;
    int _last_files = 0;
    int _first_rows;
    int _last_rows = 0;
    //std::cout << "total_rows: " << total_rows << std::endl;
    if (total_rows % delimetr) {
        if (is_strict) {
            _first_files = _total_files - 1;
            _last_files = 1;
            _first_rows = delimetr;
            _last_rows = total_rows % delimetr;
        } else {
            _first_files = total_rows % _total_files;
            _last_files = _total_files - _first_files;
            _first_rows = total_rows / _total_files + 1;
            _last_rows = total_rows / _total_files;
        }
    } else {
        _total_files = rows/delimetr;
        _first_files = _total_files;
        _first_rows = total_rows/_total_files;
    }
    return
    {
        total_files(_total_files),
        first_files(_first_files),
        last_files(_last_files),
        first_rows(_first_rows),
        last_rows(_last_rows)
    };
}
