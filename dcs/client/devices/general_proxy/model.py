from pathlib import Path

from model import Model

from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.config import GeneralProxyConfig

from utils import get_file_info

class GeneralProxyModel(Model):
    def __init__(self, session: GeneralProxySession, config: GeneralProxyConfig):
        super().__init__(session=session, config=config)

    def read_results(self, file_path: Path) -> bool:
        if not file_path.exists():
            return False

        file_info = get_file_info(file_path)
        file_info.send_name = "general_proxy"

        self._add_file(file_info)

        return True