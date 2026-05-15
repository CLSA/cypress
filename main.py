import os
import signal
import psutil
import uvicorn
import multiprocessing
import asyncio

from typing import Annotated
from enum import Enum

from fastapi import FastAPI, Path, HTTPException, Request
from fastapi import BackgroundTasks

from fastapi.middleware.httpsredirect import HTTPSRedirectMiddleware
from fastapi.middleware.trustedhost import TrustedHostMiddleware

from settings import CYPRESS_VERSION, ALLOWED_HOSTS, LOGGING_CONFIG, LOG_LEVEL

from config import config
from session import Session

from devices.device import Device

# from devices.audiometer.main import Audiometer, AudiometerSession
# from devices.blood_pressure.main import BloodPressure, BPSession
from devices.crt.main import CRT, CRTSession
from devices.cdtt.main import CDTT, CDTTSession

# from devices.frax.main import FRAX, FRAXSession
# from devices.dxa.main import DXA, DXASession

app = FastAPI()
app.add_middleware(HTTPSRedirectMiddleware)
app.add_middleware(TrustedHostMiddleware, allowed_hosts=ALLOWED_HOSTS)

# The device currently opened
current_session: dict[str, multiprocessing.Process] | None = None


class DeviceEnum(str, Enum):
    CRT = "choice_reaction_test"
    CDTT = "cdtt"
    HR = "hearing"
    GRIP = "hand_grip"
    DXA1 = "dxa1"
    DXA2 = "dxa2"
    FRAX = "frax"
    ECG = "mac5"
    ECHO = "echo"
    BP = "blood_pressure"
    RET_L = "oct_left"
    RET_R = "oct_right"
    ORA = "ora"


devices: dict[DeviceEnum, Device] = {
    # "hearcon": Audiometer,
    # "watch_bp": BloodPressure,
    DeviceEnum.CDTT: CDTT,
    DeviceEnum.CRT: CRT,
    # "dxa1": DXA,
    # "dxa2": DXA,
    # "mac5": "",
    # "vivid_iq": "",
    # "frax": FRAX,
    # "general_proxy_consent": "",
    # "hand_grip": "",
    # "oct_left": "",
    # "oct_right": "",
    # "easyone_connect": "",
    # "ora": "",
    # "weight_scale": "",
}


async def monitor_session():
    global current_session

    while current_session and psutil.pid_exists(current_session["process"].pid):
        await asyncio.sleep(1)

    current_session = None


def set_session(device: str, session: Session):
    global current_session

    device_process = multiprocessing.Process(
        target=devices[device].run, args=(session,)
    )
    current_session = {
        "id": session.session_id,
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

    return {"sessionId": session.session_id}


# @app.get("/", response_class=FileResponse)
# async def dashboard():
#     return FileResponse(path="./index.html", media_type="text/html")


@app.post("/cdtt")
async def cdtt(
    background_tasks: BackgroundTasks, session: CDTTSession, request: Request
):
    session.origin = request.headers.get("origin", None)
    return launch_device("cdtt", background_tasks, session)


@app.post("/choice_reaction_test")
async def launch(
    background_tasks: BackgroundTasks,
    session: CRTSession,
    request: Request,
):
    session.origin = request.headers.get("origin", None)
    return launch_device("choice_reaction_test", background_tasks, session)


# @app.post("/frax")
# async def frax(background_tasks: BackgroundTasks, session: FRAXSession):
#     return launch_device("frax", background_tasks, session)


# @app.post("/hearcon")
# async def audiometer(background_tasks: BackgroundTasks, session: AudiometerSession):
#     return launch_device("audiometer", background_tasks, session)


# @app.post("/dxa1")
# async def dxa1(background_tasks: BackgroundTasks, session: DXASession):
#     return launch_device("dxa", background_tasks, session)


# @app.post("/dxa2")
# async def dxa2(background_tasks: BackgroundTasks, session: DXASession):
#     return launch_device("dxa", background_tasks, session)


# @app.post("/update/")
# async def update():
#     # if current_session:
#     return {"error": "session in progress"}


@app.get("/{device}/status")
async def get_status(device: DeviceEnum):
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
    device: DeviceEnum,
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


# @app.get("/update/")
# async def update_cypress():
#     return {"updating": True}


if __name__ == "__main__":
    try:
        print(f"Cypress {CYPRESS_VERSION}\n")

        for key, value in devices.items():
            try:
                value.config.from_ini()
                print(f"[y] {key.value}")
            except Exception as e:
                print(e)
                print(f"[n] {key.value}")

        multiprocessing.freeze_support()  # for ms windows to work
        uvicorn.run(
            app=app,
            host=config.host,
            port=config.port,
            log_config=LOGGING_CONFIG,
            ssl_certfile="build/server.crt",
            ssl_keyfile="build/server.key",
        )
    except Exception as e:
        print(e)
        input("Press enter to continue..")
