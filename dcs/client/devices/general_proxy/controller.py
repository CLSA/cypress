from typing import override
from pathlib import Path

from controller import Controller

from devices.general_proxy.config import GeneralProxyConfig
from devices.general_proxy.session import GeneralProxySession
from devices.general_proxy.model import GeneralProxyModel
from devices.general_proxy.view import GeneralProxyView

from devices.general_proxy.pdf.pdf_generator import PDFGenerator


class GeneralProxyController(Controller):
    def __init__(
        self,
        session: GeneralProxySession,
        config: GeneralProxyConfig,
        model: GeneralProxyModel,
        view: GeneralProxyView,
        detached: bool = False,
        parent=None,
    ):
        super().__init__(
            parent=parent,
            session=session,
            config=config,
            model=model,
            view=view,
            detached=detached,
        )

        self.form_output_path: Path = Path(
            self.config.output_base_dir / f"{self.session.barcode}.pdf"
        )

        self.start()

    @override
    def start(self):
        self.logger.info("start")

        self.logger.info("clear results directory")
        if not self._clear_results_dir():
            self._handle_error(store_backup=True)
            return False

        self.logger.info("generating proxy form")
        generator = PDFGenerator(pdftk_exe_path=self.config.pdftk_executable)
        if not generator.prepare_form(
            form_path=self.config.form_en,
            fdf_path=self.config.fdf_en,
            input_data={"enrollmentId": self.session.uid},
            output_path=self.form_output_path,
        ):
            self.error.emit("Failed to generate proxy form")
            return False

        if not self.form_output_path.exists():
            self.error.emit("Failed to find generated proxy form")
            return False

        self.logger.info("preparing adobe")
        self.process.setProgram(str(self.config.adobe_executable.resolve()))
        self.process.setArguments([str(self.form_output_path.resolve())])
        self.process.setWorkingDirectory(str(self.config.adobe_working_dir.resolve()))

        self.logger.info("starting adobe")
        self.process.start()

        return True

    @override
    def measure(self):
        self.logger.info("reading pdf")

        if not self.form_output_path.exists():
            self.logger.error("form output path does not exist")
            self._handle_error(store_backup=True)
            return False

        if not self.model.read_results(self.form_output_path):
            self.logger.error("failed to read results")
            self._handle_error(store_backup=True)
            return False

        self.measured.emit(self.model.to_response())

        return True

    @override
    def _on_process_finished(self, *args):
        self.ready_to_measure.emit()

    @override
    def restore(self):
        super().restore()
        return self._clear_results_dir()

    def _clear_results_dir(self):
        try:
            for path in self.config.output_base_dir.iterdir():
                if path.is_file():
                    path.unlink(missing_ok=True)
            return True
        except Exception as e:
            self.logger.error(e)
            return False
