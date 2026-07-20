import re
import tempfile

from pathlib import Path

from PySide6.QtCore import QProcess

from devices.general_proxy.config import GeneralProxyConfig


class PDFGenerator:
    def __init__(self, pdftk_exe_path: Path):
        self.pdftk_exe_path = pdftk_exe_path

    def prepare_form(
        self, form_path: Path, fdf_path: Path, input_data: dict, output_path: Path
    ) -> Path:
        try:
            fdf_temp_file = None
            with tempfile.NamedTemporaryFile(mode='w', suffix='.fdf', delete=False) as temp_file:
                fdf_temp_file = temp_file

                fdf_contents: str | None = None
                with open(fdf_path) as fdf_file:
                    fdf_contents = fdf_file.read()

                for key, value in input_data.items():
                    fdf_contents = self._update_field_value(fdf_contents, key, value)

                if fdf_temp_file.write(fdf_contents) != len(fdf_contents):
                    raise Exception("Error: writing to fdf file failed")

                fdf_temp_file.flush()

            pdftk_process = QProcess()
            pdftk_process.start(str(self.pdftk_exe_path.resolve()), [
                str(form_path.resolve()),
                "fill_form",
                fdf_temp_file.name,
                "output",
                str(output_path.resolve())
            ])


            if not pdftk_process.waitForStarted():
                print("pdftk failed to start")

            if not pdftk_process.waitForFinished():
                print("pdftk failed to finish")

            errors = pdftk_process.readAllStandardError().data().decode('utf-8')
            if errors:
                raise Exception(errors)

        except Exception as e:
            print(e)
            return False

        finally:
            print("removing temp file")
            if fdf_temp_file:
                fdf_temp_file.close()
                Path(fdf_temp_file.name).unlink(missing_ok=True)

        return True

    def _update_field_value(self, fdf_contents: str, key: str, value: str):
        pattern = rf"/V (?:\(([^)]*)\)|/)\s+/T \({re.escape(key)}\)"
        replacement = f"/V ({value}) /T ({key})"

        return re.sub(pattern, replacement, fdf_contents)


if __name__ == "__main__":
    config, errors = GeneralProxyConfig.from_ini()

    generator = PDFGenerator(pdftk_exe_path=config.pdftk_executable)

    interview_id = "00000000"

    output_file = config.output_base_dir / f"{interview_id}.pdf"

    success = generator.prepare_form(
        config.form_en,
        config.fdf_en,
        { "enrollmentId": interview_id, "ProxyPostalCode": "" },
        output_file
    )

    print(success)
