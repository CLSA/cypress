import psutil
import uvicorn
import multiprocessing
import asyncio

from fastapi import FastAPI
from fastapi import BackgroundTasks
from fastapi.responses import FileResponse

from pathlib import Path

from session import Session

from instruments.audiometer.main import run_audiometer
from instruments.audiometer.session import AudiometerSession
from instruments.audiometer.config import AudiometerConfig

from instruments.cdtt.main import run_cdtt
from instruments.cdtt.session import CDTTSession
from instruments.cdtt.config import CDTTConfig

from instruments.crt.main import run_crt
from instruments.crt.session import CRTSession
from instruments.crt.config import CRTConfig

from instruments.frax.main import run_frax
from instruments.frax.session import FRAXSession
from instruments.frax.config import FRAXConfig

from instruments.dxa.main import run_dxa
from instruments.dxa.session import DXASession
from instruments.dxa.config import DXAConfig

from instruments.ecg.main import run_ecg
from instruments.ecg.session import ECGSession
from instruments.ecg.config import ECGConfig

from instruments.echo.main import run_echo
from instruments.echo.session import ECHOSession
from instruments.echo.config import ECHOConfig


# The web server
app = FastAPI()

# The instrument currently opened
current_session: dict[str, multiprocessing.Process] | None = None

devices = {
    "audiometer": run_audiometer,
    "blood_pressure": run_blood_pressure,
    "cdtt": run_cdtt,
    "choice_reaction": run_crt,
    "ecg": run_ecg,
    "frax": run_frax,
    "dxa": run_dxa,
    "grip_strength": run_grip_strength,
    "retinal_camera": run_retinal_camera,
    "spirometer": run_spirometer,
    "tonometer": run_tonometer,
    "echo": run_echo,
    "weigh_scale": run_weigh_scale,
}


async def monitor_session():
    global current_session
    while True:
        if not psutil.pid_exists(current_session["process"].pid):
            print(f"Ending session: {current_session['process'].pid}")
            current_session = None
            break
        await asyncio.sleep(1)


def set_session(device: str, session: Session):
    global current_session

    device_process = multiprocessing.Process(target=devices[device], args=(session,))
    current_session = {"device": device, "process": device_process}
    current_session["process"].start()


def is_available() -> tuple[bool, (dict | None)]:
    global current_session
    if current_session:
        return False, {
            "error": f"session in progress",
            "device": current_session["device"],
            "pid": current_session["process"].pid,
        }

    return True, None


@app.get("/", response_class=FileResponse)
async def index():
    return FileResponse(path="./index.html", media_type="text/html")


@app.post("/device/audiometer")
async def launch(background_tasks: BackgroundTasks, session: AudiometerSession):
    available, error = is_available()
    if not available:
        return error

    if not AudiometerConfig.is_device_installed():
        return {"error": "Audiometer is not installed on this workstation"}

    set_session("audiometer", session)
    background_tasks.add_task(monitor_session)
    return {"device": "audiometer", "pid": current_session["process"].pid}


@app.post("/device/cdtt")
async def launch(background_tasks: BackgroundTasks, session: CDTTSession):
    available, error = is_available()
    if not available:
        return error

    if not CDTTConfig.is_device_installed():
        return {"error": "CDTT is not installed on this workstation"}

    set_session("cdtt", session)
    background_tasks.add_task(monitor_session)
    return {"device": "cdtt", "pid": current_session["process"].pid}


@app.post("/device/choice_reaction")
async def launch(background_tasks: BackgroundTasks, session: CRTSession):
    available, error = is_available()
    if not available:
        return error

    if not CRTConfig.is_device_installed():
        return {"error": "Choice Reaction Test is not installed on this workstation"}

    set_session("choice_reaction", session)
    background_tasks.add_task(monitor_session)
    return {"device": "choice_reaction", "pid": current_session["process"].pid}


@app.post("/device/frax")
async def launch(background_tasks: BackgroundTasks, session: FRAXSession):
    available, error = is_available()
    if not available:
        return error

    if not FRAXConfig.is_device_installed():
        return {"error": "FRAX is not installed on this workstation"}

    set_session("frax", session)
    background_tasks.add_task(monitor_session)
    return {"device": "frax", "pid": current_session["process"].pid}


@app.post("/device/dxa1")
async def launch(background_tasks: BackgroundTasks, session: DXASession):
    available, error = is_available()
    if not available:
        return error

    set_session("dxa", session)
    background_tasks.add_task(monitor_session)
    return {"device": "dxa1", "pid": current_session["process"].pid}


@app.post("/device/dxa2")
async def launch(background_tasks: BackgroundTasks, session: DXASession):
    available, error = is_available()
    if not available:
        return error

    set_session("dxa", session)
    background_tasks.add_task(monitor_session)
    return {"device": "dxa2", "pid": current_session["process"].pid}


@app.post("/device/ultrasound")
async def launch(background_tasks: BackgroundTasks, session: Session):
    available, error = is_available()
    if not available:
        return error

    set_session("ultrasound", session.model_dump())
    background_tasks.add_task(monitor_session)
    return {"device": "ultrasound", "pid": current_session["process"].pid}


@app.post("/device/ecg")
async def launch(background_tasks: BackgroundTasks, session: Session):
    available, error = is_available()
    if not available:
        return error

    set_session("ecg", session.model_dump())
    background_tasks.add_task(monitor_session)
    return {"device": "ecg", "pid": current_session["process"].pid}


@app.post("/device/blood_pressure")
async def launch(background_tasks: BackgroundTasks, session: Session):
    available, error = is_available()
    if not available:
        return error

    set_session("blood_pressure", session.model_dump())
    background_tasks.add_task(monitor_session)

    return {"device": "blood_pressure", "pid": current_session["process"].pid}


@app.post("/update/")
async def update():
    # if current_session:
    return {"error": "session in progress"}


@app.get("/status/")
async def get_status():
    global current_session
    if not current_session:
        return {"status": "workstation available"}

    return {"device": current_session["device"], "pid": current_session["process"].pid}


@app.get("/update/")
async def update_cypress():
    return {"updating": True}


if __name__ == "__main__":
    test_path = Path(".")
    print("current working directory", Path.cwd())
    multiprocessing.freeze_support()  # for ms windows to work
    uvicorn.run(app, host="0.0.0.0", port=8000, log_level="info")
