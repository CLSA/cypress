import os
import signal
import psutil
import uvicorn
import multiprocessing
import asyncio
import sys
import time
import datetime

from typing import Annotated
from enum import Enum

from fastapi import FastAPI, Path, HTTPException, Request
from fastapi import BackgroundTasks
from fastapi.middleware.httpsredirect import HTTPSRedirectMiddleware
from fastapi.middleware.trustedhost import TrustedHostMiddleware

from settings import CYPRESS_VERSION, ALLOWED_HOSTS, LOGGING_CONFIG

from config import config
from session import Session

from device import Device

from devices.audiometer.main import Audiometer, AudiometerSession
from devices.blood_pressure.main import BloodPressure, BPSession
from devices.grip_strength.main import GripStrength, GripStrengthSession
from devices.crt.main import CRT, CRTSession
from devices.cdtt.main import CDTT, CDTTSession
from devices.frax.main import FRAX, FRAXSession
from devices.dxa.main import DXA, DXASession
from devices.ecg.main import ECG, ECGSession
from devices.spirometer.main import Spirometer, SpirometerSession
from devices.tonometer.main import Tonometer, TonometerSession
from devices.weigh_scale.main import WeighScale, WeighScaleSession
from devices.echo.main import ECHO, ECHOSession
from devices.general_proxy.main import GeneralProxy, GeneralProxySession

app = FastAPI()
app.add_middleware(HTTPSRedirectMiddleware)
app.add_middleware(TrustedHostMiddleware, allowed_hosts=ALLOWED_HOSTS)

# The device currently opened
current_session: dict[str, multiprocessing.Process] | None = None


class DeviceEndpoints(str, Enum):
    CRT = "choice_reaction_test"
    CDTT = "cdtt"
    HR = "hearcon"
    GRIP = "hand_grip"
    DXA = "dxa"
    DXA1 = "dxa1"
    DXA2 = "dxa2"
    FRAX = "frax"
    ECG = "mac5"
    ECHO = "echo"
    BP = "blood_pressure"
    RET_L = "oct_left"
    RET_R = "oct_right"
    ORA = "ora"
    TON = "tonometer"
    SPIRO = "spirometer"
    WT = "weigh_scale"
    GP = "general_proxy_consent"


devices: dict[DeviceEndpoints, Device] = {
    # Reception
    DeviceEndpoints.GP: GeneralProxy,
    # Interview 1
    DeviceEndpoints.HR: Audiometer,
    DeviceEndpoints.CDTT: CDTT,
    DeviceEndpoints.CRT: CRT,
    # Measure Room 1
    DeviceEndpoints.WT: WeighScale,
    DeviceEndpoints.BP: BloodPressure,
    DeviceEndpoints.SPIRO: Spirometer,
    DeviceEndpoints.DXA: DXA,
    DeviceEndpoints.DXA1: DXA,
    DeviceEndpoints.DXA2: DXA,
    DeviceEndpoints.FRAX: FRAX,
    DeviceEndpoints.ECG: ECG,
    DeviceEndpoints.ECHO: ECHO,
    # Measure Room 2
    DeviceEndpoints.GRIP: GripStrength,
    DeviceEndpoints.TON: Tonometer,
}


async def monitor_session():
    global current_session

    while current_session and psutil.pid_exists(current_session["process"].pid):
        await asyncio.sleep(1)

    current_session = None


def set_session(device: str, session: Session | None):
    global current_session

    device_process = multiprocessing.Process(
        target=devices[device].run, args=(session,)
    )
    current_session = {
        "id": session.session_id if session else None,
        "device": device,
        "process": device_process,
    }

    device_process.start()


def is_available() -> tuple[bool, (dict | None)]:
    global current_session

    if current_session:
        return False, {
            "error": f"session in progress",
            "device": current_session["device"],
            "pid": current_session["process"].pid,
        }

    return True, None


def launch_device(
    device_name: str, background_tasks: BackgroundTasks, session: Session
):
    if device_name not in devices:
        return {"error": "unsupported device"}

    available, error = is_available()
    if not available:
        return error

    device = devices[device_name]
    if not device.is_installed():
        return {"error": f"{device_name} is not installed on this workstation"}

    set_session(device_name, session)
    background_tasks.add_task(monitor_session)

    return {"sessionId": session.session_id if session else "detached"}


@app.post("/cdtt")
async def cdtt(
    background_tasks: BackgroundTasks, session: CDTTSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("cdtt", background_tasks, session)


@app.post("/choice_reaction_test")
async def choice_reaction_test(
    background_tasks: BackgroundTasks,
    session: CRTSession,
    request: Request,
):
    session.origin = request.headers.get("origin", None)
    return launch_device("choice_reaction_test", background_tasks, session)


@app.post("/hearcon")
async def hearing(
    background_tasks: BackgroundTasks, session: AudiometerSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("hearcon", background_tasks, session)


@app.post("/frax")
async def frax(
    background_tasks: BackgroundTasks, session: FRAXSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("frax", background_tasks, session)


@app.post("/dxa")
async def dxa(background_tasks: BackgroundTasks, session: DXASession, request: Request):
    session.origin = request.headers.get("origin", None)
    return launch_device("dxa", background_tasks, session)


@app.post("/dxa1")
async def dxa1(
    background_tasks: BackgroundTasks, session: DXASession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("dxa", background_tasks, session)


@app.post("/dxa2")
async def dxa2(
    background_tasks: BackgroundTasks, session: DXASession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("dxa", background_tasks, session)


@app.post("/blood_pressure")
async def blood_pressure(
    background_tasks: BackgroundTasks, session: BPSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("blood_pressure", background_tasks, session)


@app.post("/spirometer")
async def spirometer(
    background_tasks: BackgroundTasks, session: SpirometerSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("spirometer", background_tasks, session)


@app.post("/tonometer")
async def tonometer(
    background_tasks: BackgroundTasks, session: TonometerSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("tonometer", background_tasks, session)


@app.post("/general_proxy_consent")
async def general_proxy_consent(
    background_tasks: BackgroundTasks, session: GeneralProxySession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("general_proxy_consent", background_tasks, session)


@app.get("/{device}/status")
async def get_status(device: DeviceEndpoints):
    if device.value not in devices:
        raise HTTPException(status=400, detail={"error": "unsupported device"})

    global current_session
    if not current_session:
        return {"status": "available"}

    return {
        "session_id": current_session["id"],
        "in_progress": current_session["device"],
        "pid": current_session["process"].pid,
    }


@app.delete("/{device}/{session_id}")
async def end_session(
    device: DeviceEndpoints,
    session_id: Annotated[
        str,
        Path(pattern=r"^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"),
    ],
):
    if device not in devices:
        raise HTTPException(status=400, detail={"error": "unsupported device"})

    global current_session
    if not current_session:
        return

    if current_session["id"] != session_id:
        return

    pid = current_session["process"].pid
    os.kill(pid, signal.SIGTERM)


@app.get("/update/")
async def update_cypress():
    return {"updating": True}


def get_last_modified():
    exe_path = sys.executable
    mod_time = os.stat(exe_path).st_mtime
    last_modified = datetime.datetime.fromtimestamp(mod_time)
    return last_modified.strftime("%Y-%m-%d %H:%M:%S")


@app.get("/status")
async def status():
    global current_session
    global standalone_session

    response = {
        "version": CYPRESS_VERSION,
        "last_updated": get_last_modified(),
        "installed": set(
            {
                device_name
                for device_name, device in devices.items()
                if device.is_installed()
            }
        ),
        "not_installed": {
            device_name: device.get_install_errors()
            for device_name, device in devices.items()
            if not device.is_installed()
        },
    }

    if current_session:
        response["session"] = {
            "session_id": current_session["id"],
            "in_progress": current_session["device"],
            "pid": current_session["process"].pid,
        }

    return response


def restart_app():
    time.sleep(1)

    os.kill(os.getpid(), signal.SIGINT)


@app.post("/restart")
async def restart(background_tasks: BackgroundTasks):
    background_tasks.add_task(restart_app)
    return "restart"


@app.post("/detached/{device}")
async def standalone(device: str, background_tasks: BackgroundTasks):
    return launch_device(device, background_tasks, session=None)


if __name__ == "__main__":
    multiprocessing.freeze_support()  # for ms windows to work
    uvicorn.run(
        app=app,
        host=config.host,
        port=config.port,
        log_config=LOGGING_CONFIG,
        ssl_certfile=config.ssl_certfile,
        ssl_keyfile=config.ssl_keyfile,
    )
