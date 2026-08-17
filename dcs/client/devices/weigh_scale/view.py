import json

from typing import override
from view import View, State

from PySide6.QtWidgets import QMessageBox
from PySide6.QtCore import Signal

from devices.weigh_scale.session import WeighScaleSession
from devices.weigh_scale.config import WeighScaleConfig
from devices.weigh_scale.weight_measurements_widget import WeightMeasurementsWidget


class WeighScaleView(View):
    values_changed = Signal(list)
    delete_measure = Signal(int)

    def __init__(
        self,
        session: WeighScaleSession,
        config: WeighScaleConfig,
        detached: bool = False,
        parent=None,
    ):
        self.measurement_form = WeightMeasurementsWidget(config=config)

        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )
        self.session_widget.deviceStatusValue.setText("Weight")
        self.measurement_table_widget.deleteLater()

        self.measure_button.setVisible(True)
        self.measure_button.setEnabled(True)
        self.manual_entry_button.setVisible(True)
        self.manual_entry_button.setEnabled(True)

        self.start_button.setVisible(False)

        self.layout().addWidget(self.measurement_form)

    @override
    def _get_button_references(self):
        super()._get_button_references()

        self.measure_button = self.measurement_form.measureButton
        self.submit_button = self.measurement_form.submitButton
        self.manual_entry_button = self.measurement_form.manualEntryButton
        self.zero_button = self.measurement_form.zeroButton

    @override
    def _connect_signals(self):
        self.session_widget.startButton.clicked.connect(self._on_start_button_clicked)
        self.submit_button.clicked.connect(self._on_submit_button_clicked)
        self.measure_button.clicked.connect(self._on_measure_button_clicked)
        self.manual_entry_button.clicked.connect(self._on_manual_entry_clicked)
        self.measurement_form.values_changed.connect(self._on_values_changed)
        self.measurement_form.delete_measure.connect(self._on_delete_measure)

    @override
    def _on_manual_entry_clicked(self):
        #super()._on_manual_entry_clicked()

        self.manual_entry.emit([])

    def _on_values_changed(self, data):
        self.values_changed.emit(data)

    def _on_delete_measure(self, index):
        self.delete_measure.emit(index)

    @override
    def on_measured(self, output: dict):
        self.logger.debug(f"view: {output}")
        self.measurement_form.on_measured(output)

    @override
    def on_ready_to_measure(self):
        self.state = State.READY_TO_MEASURE
        self.logger.info("ready to measure")
        self.session_widget.statusValue.setText("Ready to measure")
        self.measure_button.setEnabled(True)
        self.submit_button.setEnabled(False)

    @override
    def on_manual_entry(self):
        self.session_widget.statusValue.setText(f"Manual entry")
        self.session_widget.startButton.setEnabled(False)
        self.start_button.setEnabled(False)
        self.zero_button.setEnabled(False)
        self.manual_entry_button.setEnabled(False)
        self.measurement_form.set_enabled(True)
        self.measure_button.setEnabled(False)
        self.state = State.MANUAL_ENTRY

    @override
    def on_ready_to_submit(self, ready: bool):
        if ready:
            self.state = State.READY_TO_SUBMIT
            self.logger.info("ready to submit")
            self.session_widget.statusValue.setText("Ready to submit")
            self.submit_button.setEnabled(True)
        else:
            self.submit_button.setEnabled(False)
            self.session_widget.statusValue.setText("Ready to measure")