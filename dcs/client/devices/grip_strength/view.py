from typing import override
from PySide6.QtWidgets import QTableWidgetItem, QHeaderView, QAbstractItemView

from view import View


from devices.grip_strength.config import GripStrengthConfig
from devices.grip_strength.session import GripStrengthSession

from devices.grip_strength.grip_stats import GripStatsWidget

class GripStrengthView(View):
    def __init__(
        self,
        session: GripStrengthSession,
        config: GripStrengthConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(parent=parent, session=session, detached=detached, config=config)

        self.session_widget.deviceStatusValue.setText("Grip Strength")

        self.measure_button.setVisible(True)
        self.start_button.setVisible(True)


    @override
    def _get_button_references(self):
        self.measure_button = self.grip_widget.measure_button
        self.submit_button = self.grip_widget.submit_button
        self.manual_entry_button = self.grip_widget.manual_entry_button
        self.start_button = self.session_widget.startButton

    @override
    def _add_measurement_table_widget(self):
        self.grip_widget = GripStatsWidget(parent=self)
        self.layout().addWidget(self.grip_widget)

    @override
    def on_ready_to_measure(self):
        super().on_ready_to_measure()
        self.session_widget.statusValue.setText("Press measure once complete")

    @override
    def on_measured(self, output: dict):
        output = output["value"]

        unit = output["metadata"]["units"]

        self.grip_widget.test_value.setText(output["metadata"]["test"])
        self.grip_widget.primary_stat_value.setText(output["metadata"]["primary_stat"])
        self.grip_widget.sequence_value.setText(output["metadata"]["sequence"])
        self.grip_widget.rung_value.setText(str(output["metadata"]["rung"]))
        self.grip_widget.max_reps_value.setText(str(output["metadata"]["max_reps"]))
        self.grip_widget.avg_value.setText(f"{round(output["metadata"]["average"], 2)} {unit}")
        self.grip_widget.max_value.setText(f"{round(output["metadata"]["maximum"], 2)} {unit}")
        self.grip_widget.cv_value.setText(str(output["metadata"]["cv"]))
        self.grip_widget.rep1_value.setText(f"{round(output["results"][0]["rep1"]["value"], 2)} {unit}")
        self.grip_widget.rep2_value.setText(f"{round(output["results"][0]["rep2"]["value"], 2)} {unit}")
        self.grip_widget.rep3_value.setText(f"{round(output["results"][0]["rep3"]["value"], 2)} {unit}")

        # self.table.clear()

        # self.table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)
        # self.table.setEditTriggers(QAbstractItemView.NoEditTriggers)
        # self.table.setRowCount(len(output["value"]["results"]))
        # self.table.setColumnCount(len(self.columns))
        # self.table.setHorizontalHeaderLabels(self.columns)

        # for index, result in enumerate(output["value"]["results"]):
        #     rep1 = QTableWidgetItem(f"{round(result["rep1"]["value"], 2)} {result["rep1"]["units"]}")
        #     rep2 = QTableWidgetItem(f"{round(result["rep2"]["value"], 2)} {result["rep2"]["units"]}")
        #     rep3 = QTableWidgetItem(f"{round(result["rep3"]["value"], 2)} {result["rep3"]["units"]}")

        #     self.table.setItem(index, 0, rep1)
        #     self.table.setItem(index, 1, rep2)
        #     self.table.setItem(index, 2, rep3)

        super().on_measured()
