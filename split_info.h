#ifndef SPLIT_INFO_H
#define SPLIT_INFO_H


struct total_files {
    explicit total_files( int new_value) {
        value = new_value;
    }
    int value;
};
struct first_files {
    explicit first_files( int new_value) {
        value = new_value;
    }
    int value;
};
struct last_files {
    explicit last_files( int new_value) {
        value = new_value;
    }
    int value;
};
struct first_rows {
    explicit first_rows( int new_value) {
        value = new_value;
    }
    int value;
};
struct last_rows {
    explicit last_rows( int new_value) {
        value = new_value;
    }
    int value;
};

struct split_info {
    split_info() = default ;
    split_info (total_files t_files,
                first_files f_files,
                last_files l_files,
                first_rows f_rows,
                last_rows l_rows) :

                total_files(t_files.value),
                first_files(f_files.value),
                last_files(l_files.value),
                first_rows(f_rows.value),
                last_rows(l_rows.value)
    {
    }

    int total_files;
    int first_files;
    int last_files;
    int first_rows;
    int last_rows;
};

#endif // SPLIT_INFO_H

