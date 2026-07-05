from typing import override
from view import View

from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.config import GeneralProxyConfig


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

    @override
    def on_measured(self, output: dict):
        super().on_measured(output)
