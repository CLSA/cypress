DEVICE_NAME = "spirometer"
LOG_LOCATION = "logs/spirometer.log"
LOG_MAX_SIZE_MB = 10 * 1024 * 1024
LOG_BACKUPS = 1
LOG_LEVEL = "INFO"

LOG_CONFIG = {
    "version": 1,
    "disable_existing_loggers": False,
    "formatters": {
        "default": {
            "format": f"[%(asctime)s] %(levelname)s - %(message)s",
            "datefmt": "%d/%m/%Y %I:%M:%S",
        },
    },
    "handlers": {
        "main_log": {
            "class": "logging.handlers.RotatingFileHandler",
            "filename": "logs/main.log",
            "formatter": "default",
            "maxBytes": LOG_MAX_SIZE_MB,
            "backupCount": LOG_BACKUPS,
        },
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