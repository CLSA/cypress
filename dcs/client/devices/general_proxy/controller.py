from controller import Controller

from devices.general_proxy.config import GeneralProxyConfig
from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.model import GeneralProxyModel
from devices.general_proxy.view import GeneralProxyView


class GeneralProxyController(Controller):
    def __init__(
        self,
        session: GeneralProxySession,
        config: GeneralProxyConfig,
        model: GeneralProxyModel,
        view: GeneralProxyView,
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
