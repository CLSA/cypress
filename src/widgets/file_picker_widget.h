#ifndef FILE_PICKER_WIDGET_H
#define FILE_PICKER_WIDGET_H

#include <QWidget>

namespace Ui {
class FilePickerWidget;
}

class FilePickerWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool isFile READ getIsFile WRITE setIsFile NOTIFY isFileChanged)
    Q_PROPERTY(bool isDir READ getIsDir WRITE setIsDir NOTIFY isDirChanged)
    Q_PROPERTY(bool isExe READ getIsExe WRITE setIsExe NOTIFY isExeChanged)

public:
    explicit FilePickerWidget(QWidget *parent = nullptr);
    ~FilePickerWidget();

    QString pickFile();

    void setPath(QString path);
    void setTitle(QString name);

    bool getIsFile();
    bool getIsDir();
    bool getIsExe();

    void setIsFile(bool value);
    void setIsDir(bool value);
    void setIsExe(bool value);

    // check if the picked file/dir/exe is valid (exists, readable & writable)
    bool validate(const QString& path);

signals:
    void pathChanged(QString path);

    void isFileChanged();
    void isDirChanged();
    void isExeChanged();

private slots:
    void on_pathButton_clicked();

private:
    bool m_isFile { false };
    bool m_isDir { false };
    bool m_isExe { false };

    QString m_path;
    Ui::FilePickerWidget *ui;
};

#endif // FILE_PICKER_WIDGET_H
