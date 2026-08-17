import psutil
import json

from pathlib import Path
from dataclasses import dataclass

from PySide6.QtCore import Qt, QLocale
from PySide6.QtWidgets import (
    QFileDialog,
    QStyledItemDelegate,
    QSpinBox,
    QDoubleSpinBox,
    QComboBox
)

@dataclass(kw_only=True)
class FileInfo:
    name: str
    raw_size: int
    readable_size: str
    file_name: str
    file_path: Path
    extension: str

    # pine preferred name (no extension)
    send_name: str | None

@dataclass(kw_only=True)
class DicomFileInfo(FileInfo):
    patient_id: str
    body_part_examined: str
    modality: str
    laterality: str
    study_id: str
    media_storage_uid: str
    study_date: str
    series_number: str
    instance_number: str


def get_file_size(file_path: Path):
    size_bytes = file_path.stat().st_size

    units = ["B", "KB", "MB", "GB", "TB", "PB", "EB"]
    for unit in units:
        if abs(size_bytes) < 1024.0:
            return f"{size_bytes:.2f}{unit}"
        size_bytes /= 1024.0


def get_file_info(file_path: Path):
    if not file_path:
        raise ValueError("file_path parameter is missing")

    if not file_path.exists():
        raise Exception(f"{file_path.name} does not exist")

    if not file_path.is_file():
        raise Exception(f"{file_path.name} is not a file")

    return FileInfo(
        name=file_path.name.partition(".")[0],
        raw_size=file_path.stat().st_size,
        readable_size=get_file_size(file_path),
        file_name=file_path.name,
        extension="".join(file_path.suffixes), # ignore period
        file_path=file_path,
        send_name=None
    )

def file_dialog_save(data: dict, filename: str, ext: str):
    file_path, selected_filter = QFileDialog.getSaveFileName(
        None,
        "Select output file",
        str(
            (
                Path.home() / f"{filename}.{ext}"
            ).resolve()
        ),
        "JSON files (*.json)",
    )
    if file_path:
        with open(file_path, "w") as file:
            json.dump(data, file, indent=4)
    else:
        return False


def is_process_running(process_name: str):
    for process in psutil.process_iter(["name"]):
        if process_name.lower() == process.info["name"].lower():
            return True
    return False

def stop_process(process_name: str) -> bool:
    for process in psutil.process_iter(["name"]):
        if process_name.lower() == process.info["name"].lower():
            process.kill()
            return True
    return False


def clear_directory(directory: Path):
    if not directory:
        raise ValueError(f"invalid directory")

    if not directory.exists():
        raise ValueError("directory does not exist")

    if not directory.is_dir():
        raise ValueError("path is not a directory")

    for path in directory.iterdir():
        if path.is_file():
            path.unlink(missing_ok=True)

class IntegerSpinBoxDelegate(QStyledItemDelegate):

    def __init__(self, min_val, max_val, parent=None):
        super().__init__(parent)
        self.min_val = min_val
        self.max_val = max_val

    def createEditor(self, parent, option, index):
        editor = QSpinBox(parent)
        editor.setRange(self.min_val, self.max_val)
        editor.setAutoFillBackground(True)

        return editor

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.EditRole)
        editor.setValue(int(value) if value else self.min_val)

    def setModelData(self, editor, model, index):
        editor.interpretText()
        value = editor.value()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)

class ComboBoxDelegate(QStyledItemDelegate):

    def __init__(self, options, parent=None):
        super().__init__(parent)
        self.options = options

    def createEditor(self, parent, option, index):
        editor = QComboBox(parent)
        editor.addItems(self.options)
        editor.setAutoFillBackground(True)

        return editor

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.EditRole)
        editor.setCurrentText(value)

    def setModelData(self, editor, model, index):
        value = editor.currentText()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)


class DoubleSpinBoxDelegate(QStyledItemDelegate):

    def __init__(self, min_val, max_val, decimals=2, locale: str = False, parent=None):
        super().__init__(parent)
        self.min_val = min_val
        self.max_val = max_val
        self.decimals = decimals
        self.locale = locale

    def createEditor(self, parent, option, index):
        editor = QDoubleSpinBox(parent)
        if self.locale == 'fr':
            editor.setLocale(QLocale(QLocale.French, QLocale.Canada))
        editor.setRange(self.min_val, self.max_val)
        editor.setDecimals(self.decimals)
        editor.setAutoFillBackground(True)

        return editor

    def setEditorData(self, editor, index):
        value = index.model().data(index, Qt.EditRole)
        editor.setValue(float(value) if value else self.min_val)

    def setModelData(self, editor, model, index):
        editor.interpretText()
        value = editor.value()
        model.setData(index, value, Qt.EditRole)

    def updateEditorGeometry(self, editor, option, index):
        editor.setGeometry(option.rect)