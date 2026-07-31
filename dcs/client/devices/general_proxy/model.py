from pathlib import Path

from pyhanko.pdf_utils.reader import PdfFileReader

from model import Model


from devices.general_proxy.settings import logger


class GeneralProxyModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

    #def has_signature(self) -> bool:
    #    if not len(self.files):
    #        logger.critical("No files found")
    #        return False

    #    form_pdf_info = self.files[0]
    #    num_signatures = 0

    #    with open(form_pdf_info.file_path, "rb") as form:
    #        reader = PdfFileReader(form)
    #        num_signatures = sum(
    #            [
    #                len(reader.embedded_signatures),
    #                len(reader.embedded_regular_signatures),
    #                len(reader.embedded_timestamp_signatures),
    #            ]
    #        )

    #    return num_signatures > 0

    def read_results(self, file_path: Path) -> bool:
        added = self.add_file(file_path=file_path, send_name="general_proxy")
        if not added:
            logger.critical("Error adding file")
            return False

        return True
