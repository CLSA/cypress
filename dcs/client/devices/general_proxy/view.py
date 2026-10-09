from typing import override
from view import View, State

from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.config import GeneralProxyConfig

from PySide6.QtWidgets import QSizePolicy


class GeneralProxyView(View):
    def __init__(
        self,
        session: GeneralProxySession,
        config: GeneralProxyConfig,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent, session=session, config=config, detached=detached
        )

        self.session_widget.deviceStatusValue.setText("General Proxy")
        self.session_widget.setSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)

        self.start_button.setVisible(False)
        self.measurement_table_widget.setVisible(False)

        #self.measurement_table_widget.deleteLater()
        self.setFixedSize(400, 200)

        self.layout().addStretch()

    @override
    def on_ready_to_measure(self):
        #super().on_ready_to_measure()
        self.measure.emit()

    @override
    def on_measured(self):
        super().on_measured()
        self.submit.emit()

    @override
    def on_submitting(self):
        super().on_submitting()

    @override
    def on_submitted(self):
        super().on_submitted()
        self.close()
