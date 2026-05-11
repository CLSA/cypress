import logging.config

DEVICE_NAME = "choice_reaction"
LOG_FILENAME = "logs/crt.log"

logging.config.dictConfig({
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
            "class": "logging.FileHandler",
            "filename": LOG_FILENAME,
            "formatter": "default",
        },

    },
    "loggers": {
        DEVICE_NAME: {
            "handlers": ["file_handler"], "level": "INFO"
        }
    },
})

logger = logging.getLogger(DEVICE_NAME)