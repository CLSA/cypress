from model import Model

from devices.echo.session import ECHOSession
from devices.echo.config import ECHOConfig


class ECHOModel(Model):
    def __init__(self, session: ECHOSession, config: ECHOConfig):
        super().__init__(session=session, config=config)
