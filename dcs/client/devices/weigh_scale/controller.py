import datetime
import json

import traceback
from typing import override

from PySide6.QtCore import Signal, QByteArray
from PySide6.QtSerialPort import QSerialPort, QSerialPortInfo
from PySide6.QtWidgets import QMessageBox

from controller import Controller

from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.model import WeighScaleModel
from devices.weigh_scale.view import WeighScaleView


class WeighScaleController(Controller):
    devices_scanned = Signal()

    def __init__(
        self,
        session: WeighScaleSession,
        config: WeighScaleConfig,
        model: WeighScaleModel,
        view: WeighScaleView,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            view=view,
            detached=detached,
        )

        self.ports_available = {}
        self.read_buffer: bytearray = bytearray()

        self.view.zero_button.clicked.connect(self.zero_device)
        self.view.manual_entry.connect(self._on_manual_entry)
        self.view.values_changed.connect(self._on_values_changed)
        self.view.delete_measure.connect(self._on_delete_measure)

        self.start()

    @override
    def start(self):
        self.port: QSerialPort = QSerialPort()

        self.port.setPortName(self.config.port_name)
        self.port.setBaudRate(9600)
        self.port.setDataBits(QSerialPort.DataBits.Data8)
        self.port.setParity(QSerialPort.Parity.NoParity)
        self.port.setStopBits(QSerialPort.StopBits.OneStop)
        self.port.setFlowControl(QSerialPort.FlowControl.NoFlowControl)

        self.port.readyRead.connect(self._read_device)
        self.port.errorOccurred.connect(self._handle_serial_port_error)

        self.logger.info("opening")
        if not self.port.open(QSerialPort.OpenModeFlag.ReadWrite):
            self.logger.error(self.port.errorString())
            return False

        opened = self.port.isOpen()
        self.logger.info(f"port opened: {opened}")
        if not opened:
            return False

        self.logger.info("zeroing...")
        self._write_device(b"z")

        self.ready_to_measure.emit()

    @override
    def measure(self):
        self.logger.info("measure requested")
        self._write_device(b"p")

    def zero_device(self):
        self.logger.info("zero requested")
        self._write_device(b"z")

    def _write_device(self, data: bytes):
        self.read_buffer.clear()
        bytes_written = self.port.write(data)
        flushed = self.port.flush()
        self.logger.debug(f"bytes_written: {bytes_written} flushed: {flushed}")

    def _read_device(self):
        data = bytes(self.port.readAll())
        self.read_buffer.extend(data)

        self.logger.debug(repr(self.read_buffer))

        if not self.read_buffer.endswith(b"\r\n"):
            return

        response = self.read_buffer.decode("ascii").strip()
        self.logger.debug(response)

        try:
            response_parts = response.split(" ")

            self.logger.debug(response_parts)

            weight = float(response_parts[0])
            unit = str(response_parts[1])
            mode = str(response_parts[2])

            self.logger.debug(weight, unit, mode)

            added, error = self.model.add_measurement(weight, unit, mode)
            if not added and error == "Weight is zero":
                self._show_message_box(
                    title="Invalid weight", msg="Weight is zero", level="warning"
                )
                return
            elif not added:
                self.logger.error(error)
                self._show_message_box(
                    title="Error",
                    msg=error,
                    level="warning",
                )
            else:
                self.logger.debug("measured")
                self.measured.emit(self.model.to_response())

        except Exception as e:
            traceback.print_exc()
            self.logger.error(e)

        self.logger.debug("checking is valid")
        is_valid = self.model.is_valid()

        self.ready_to_submit.emit(is_valid)

        self.read_buffer.clear()

    def _on_manual_entry(self, data):
        if (
            not self.model.manual_entry
            and self.model.test
            and len(self.model.test.measures)
        ):
            btn = self._show_message_box(
                title="Entering manual entry mode",
                msg="Automatic measurements exist, are you sure you'd like to enter manual entry? This will remove existing measurements",
                level="warning",
                allow_cancel=True,
            )
            if btn == QMessageBox.StandardButton.Ok:
                self.model.set_manual_entry()
                self.view.on_manual_entry()
                self.measured.emit(self.model.to_response())
        else:
            self.view.on_manual_entry()
            self.measured.emit(self.model.to_response())

    def _on_values_changed(self, data):
        self.model.set_manual_entry_data(data)
        self.measured.emit(self.model.to_response())
        self.ready_to_submit.emit(self.model.is_valid())

    def _on_delete_measure(self, measure_index):
        self.model.delete_measurement(measure_index)
        self.measured.emit(self.model.to_response())
        self.ready_to_submit.emit(self.model.is_valid())

    def _handle_serial_port_error(self, error: QSerialPort.SerialPortError):
        if error != QSerialPort.SerialPortError.NoError:
            self.logger.error(f"{error}, {self.port.errorString()}")

    def _handle_data_terminal_ready_changed(self, set: bool):
        self.logger.info(
            f"handle_data_terminal_ready_changed: {'high' if set else 'low'}"
        )

    def _handle_request_to_send_changed(self, set: bool):
        self.logger.info(f"handle_request_to_send_changed: {'high' if set else 'low'}")
