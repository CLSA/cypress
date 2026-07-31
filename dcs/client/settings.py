import base64

from config import config

CYPRESS_VERSION = "2.0.0"

ALLOWED_HOSTS = [host.strip() for host in config.allowed_hosts.split(",")]

PINE_AUTH_TOKEN = f"Basic {base64.b64encode(config.auth.encode("utf-8")).decode("utf-8")}"
TRANSFER_CHUNK_SIZE = 8192
TRANSFER_SLEEP_TIME = 0.01

LOG_MAX_SIZE_MB = 10 * 1024 * 1024
LOG_NUM_BACKUPS = 1
LOG_LEVEL = "DEBUG"

LOGGING_CONFIG = {
    "version": 1,
    "disable_existing_loggers": False,
    "formatters": {
        "default": {
            "format": "[%(asctime)s] %(levelname)s - %(message)s",
            "datefmt": "%d/%m/%Y %I:%M:%S",
        },
    },
    "handlers": {
        "console": {
            "class": "logging.StreamHandler",
            "formatter": "default",
        },
        "file_handler": {
            "class": "logging.handlers.RotatingFileHandler",
            "filename": "logs/server.log",
            "formatter": "default",
            'maxBytes': LOG_MAX_SIZE_MB,
            'backupCount': LOG_NUM_BACKUPS,
        },
    },
    "loggers": {
        "uvicorn": {"handlers": ["file_handler", "console"], "level": LOG_LEVEL},
    },
}