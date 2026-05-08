import psutil
import uvicorn
import multiprocessing
import asyncio

from fastapi import FastAPI, Request, status
from fastapi import BackgroundTasks
from fastapi.responses import FileResponse, JSONResponse

from config import CypressConfig
from session import Session

from devices.audiometer import run_audiometer, AudiometerSession, AudiometerConfig
from devices.cdtt import run_cdtt, CDTTSession, CDTTConfig
from devices.crt import run_crt, CRTSession, CRTConfig
from devices.frax import run_frax, FRAXSession, FRAXConfig
from devices.dxa import run_dxa, DXASession, DXAConfig
from devices.ecg import run_ecg, ECGSession, ECGConfig
from devices.echo import run_echo, ECHOSession, ECHOConfig
from devices.blood_pressure import run_blood_pressure, BPSession, BPConfig

from devices.grip_strength import (
    run_grip_strength,
    GripStrengthSession,
    GripStrengthConfig,
)

print(f"Cypress v2.0.0")
config = CypressConfig.from_ini("config.ini", "cypress")
ALLOWED_IPS = [ip.strip() for ip in config.allowed_ips.split(",")]

# The web server
app = FastAPI()

# The device currently opened
current_session: dict[str, multiprocessing.Process] | None = None

devices = {
    # "audiometer": run_audiometer,
    # "blood_pressure": run_blood_pressure,
    "cdtt": run_cdtt,
    "choice_reaction": run_crt,
    "ecg": run_ecg,
    "frax": run_frax,
    "dxa": run_dxa,
    # "grip_strength": run_grip_strength,
    # "retinal_camera": run_retinal_camera,
    # "spirometer": run_spirometer,
    # "tonometer": run_tonometer,
    # "echo": run_echo,
    # "weigh_scale": run_weigh_scale,
}


@app.middleware("http")
async def validate_ip(request: Request, call_next):
    ip = str(request.client.host)

    if ip not in ALLOWED_IPS:
        data = {"message": f"IP {ip} is not allowed to access this resource."}
        return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST, content=data)

    return await call_next(request)


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
    multiprocessing.freeze_support()  # for ms windows to work
    uvicorn.run(
        app, host=str(config.host), port=config.port, log_level=config.log_level
    )
