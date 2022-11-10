#include<file_info.h>


QPair<QString, QString> getFileInfo( QString& filePath)
{
    QPair<QString, QString> result;
    if(!QFileInfo(filePath).isReadable()) {
        return {"error","Corrupted file path or file not readable"};
    }
    QFileInfo f(filePath);
    result.first = f.suffix();

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(filePath, QMimeDatabase::MatchContent);
    result.second = mime.name();

    return result;
}

