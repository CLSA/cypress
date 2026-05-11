from config import config

ALLOWED_IPS = [ip.strip() for ip in config.allowed_ips.split(",")]
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
            "class": "logging.FileHandler",
            "filename": "logs/server.log",
            "formatter": "default",
        },
    },
    "loggers": {
        "uvicorn": {"handlers": ["console", "file_handler"], "level": "INFO"},
    },
}
