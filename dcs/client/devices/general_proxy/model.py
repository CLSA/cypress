import logging
from pathlib import Path

from devices.general_proxy.pdf.pdf_generator import PDFGenerator
from devices.general_proxy.config import GeneralProxyConfig

from model import Model



from pyhanko.pdf_utils.reader import PdfFileReader


logger = logging.getLogger("general_proxy")


class GeneralProxyModel(Model):
    def __init__(self, session, config):
        super().__init__(session, config)

    def read_results(self, file_path: Path) -> bool:
        self.reset()

        added = self.add_file(
            file_path=file_path, send_name="general_proxy", ext=".pdf"
        )
        if not added:
            logger.error("could not add general_proxy.pdf")
            return False

        form_results = self._parse_form(file_path)
        if form_results is None:
            logger.error("no form data found")
            return False

        self._set_results(form_results)

        if not self.has_signature():
            logger.warning("pdf has not been signed")

        return True

    def has_signature(self) -> bool:
        if not len(self.files):
            logger.error("has_signature: no files found")
            return False

        try:
            form_pdf_info = self.files[0]
            num_signatures = 0

            with open(form_pdf_info.file_path, "rb") as form:
                reader = PdfFileReader(form)
                num_signatures = sum(
                    [
                        len(reader.embedded_signatures),
                        len(reader.embedded_regular_signatures),
                        len(reader.embedded_timestamp_signatures),
                    ]
                )
            return num_signatures > 0

        except Exception as e:
            logger.error(f"has_signature: {e}")
            return False

    def _parse_form(self, file_path) -> dict:
        try:
            form_data = PDFGenerator(
                pdftk_exe_path=self.config.pdftk_executable
            ).dump_fields(file_path)

            if not form_data:
                logger.error(f"dump_fields returned '{form_data}'")
                return None

            form_fields = self._parse_form_data(form_data)

            return form_fields

        except Exception as e:
            logger.error(e)
            return None

    def _parse_form_data(self, form_data: str) -> dict:
        try:
            fields = []

            raw_fields = form_data.split("---")
            for raw_field in raw_fields:
                raw_attrs = raw_field.replace("\r", "").split("\n")
                if not raw_attrs:
                    continue
                field = {}
                for raw_attr in raw_attrs:
                    attr = raw_attr.strip()
                    if attr:
                        key, value = attr.split(":")
                        field[key] = value.strip()
                if field:
                    fields.append(field)

            return fields
        except Exception as e:
            logger.error(e)
            return None

    def _set_results(self, parsed_form_fields: list):
        self.results = parsed_form_fields

        for parsed_field in parsed_form_fields:
            field_name = parsed_field.get("FieldName")
            field_type = parsed_field.get("FieldType")
            field_name_alt = parsed_field.get("FieldNameAlt")
            field_value = parsed_field.get("FieldValue")

            logger.debug(f"{field_name_alt}  - {field_value}")

            if field_name is None:
                logger.error(f"field name does not exist - {field_name_alt}")
                continue

            if not field_name:
                logger.error(f"field name is empty")
                continue

            self.metadata[field_name.replace(".", "_")] = self._convert_type(
                field_type, field_value
            )

    def _convert_type(self, field_type: str, field_value: str):
        if field_type == "Button":
            return (
                True if field_value == "Yes" else False if field_value == "No" else None
            )

        if field_type == "Text":
            return field_value

        return None



if __name__ == '__main__':
    import json

    file_paths = Path("C:/Users/hoarea/cypress/analysis/SHER_CONSENT_GP/fix")

    config, errors = GeneralProxyConfig.from_ini()


    for uid_folder in file_paths.iterdir():
        if uid_folder.is_dir():
            report = uid_folder / "general_proxy.pdf"

            model = GeneralProxyModel(None, config)
            model.read_results(report)

            with open(uid_folder / "response.json", "w") as json_file:
                json.dump(model.to_response(), json_file, indent=2)

