DEVICE_NAME = "dxa"
LOG_LOCATION = f"logs/{DEVICE_NAME}.log"
LOG_MAX_SIZE_MB = 10 * 1024 * 1024
LOG_BACKUPS = 1
LOG_LEVEL = "INFO"

LOG_CONFIG = {
    "version": 1,
    "disable_existing_loggers": False,
    "formatters": {
        "default": {
            "format": f"[%(asctime)s] %(levelname)s - {DEVICE_NAME} - %(message)s",
            "datefmt": "%d/%m/%Y %I:%M:%S",
        },
    },
    "handlers": {
        "file_handler": {
            "class": "logging.handlers.RotatingFileHandler",
            "filename": LOG_LOCATION,
            "formatter": "default",
            "maxBytes": LOG_MAX_SIZE_MB,
            "backupCount": LOG_BACKUPS,
        },
        "console": {
            "class": "logging.StreamHandler",
            "formatter": "default",
        },
    },
    "loggers": {
        DEVICE_NAME: {"handlers": ["console", "file_handler"], "level": LOG_LEVEL}
    },
}