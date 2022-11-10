#ifndef FILE_INFO_H
#define FILE_INFO_H

#include<QFileInfo>
#include<QPair>
#include<QString>
#include<QMimeDatabase>

QPair<QString, QString> getFileInfo( QString& filePath);

#endif // FILE_INFO_H
