from model import Model

from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.config import GeneralProxyConfig

class GeneralProxyModel(Model):
    def __init__(self, session: GeneralProxySession, config: GeneralProxyConfig):
        super().__init__(session=session, config=config)
