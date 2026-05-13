import os
import signal
import psutil
import uvicorn
import multiprocessing
import asyncio

from typing import Annotated

from fastapi import FastAPI, Path, HTTPException
from fastapi import BackgroundTasks
from fastapi.responses import FileResponse

from fastapi.middleware.httpsredirect import HTTPSRedirectMiddleware
from fastapi.middleware.trustedhost import TrustedHostMiddleware

from settings import ALLOWED_HOSTS, logger

from config import config
from session import Session

from devices.device import Device
from devices.audiometer.main import Audiometer, AudiometerSession
from devices.blood_pressure.main import BloodPressure, BPSession
from devices.crt.main import CRT, CRTSession
from devices.cdtt.main import CDTT, CDTTSession
from devices.frax.main import FRAX, FRAXSession
from devices.dxa.main import DXA, DXASession

app = FastAPI()
app.add_middleware(HTTPSRedirectMiddleware)
app.add_middleware(TrustedHostMiddleware, allowed_hosts=ALLOWED_HOSTS)

# The device currently opened
current_session: dict[str, multiprocessing.Process] | None = None

devices: dict[str, Device] = {
    #"hearcon": Audiometer,
    #"watch_bp": BloodPressure,
    #"cdtt": CDTT,
    "choice_reaction_test": CRT,
    #"dxa1": DXA,
    #"dxa2": DXA,
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
    while True:
        if not psutil.pid_exists(current_session["process"].pid):
            current_session = None
            break
        await asyncio.sleep(1)


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

    print("sending session id", session.session_id)
    print("sending session id", session.session_id)
    print("sending session id", session.session_id)

    return {"sessionId": session.session_id}


# @app.get("/", response_class=FileResponse)
# async def dashboard():
#     return FileResponse(path="./index.html", media_type="text/html")


# @app.post("/cdtt")
# async def cdtt(background_tasks: BackgroundTasks, session: CDTTSession):
#     return launch_device("cdtt", background_tasks, session)


@app.post("/choice_reaction_test")
async def crt(background_tasks: BackgroundTasks, session: CRTSession):
    logger.info("launch", session.session_id)
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
async def get_status(device: str):
    if device not in devices:
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
    device: str,
    session_id: Annotated[
        str,
        Path(pattern=r"^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"),
    ],
):
    print(f"ending session {session_id}")
    if device not in devices:
        raise HTTPException(status=400, detail={"error": "unsupported device"})

    print(f"ending session {session_id}")

    global current_session
    if not current_session:
        return

    print(current_session['id'], session_id)
    if current_session["id"] != session_id:
        return

    print(f"ending session {session_id}")
    pid = current_session["process"].pid
    os.kill(pid, signal.SIGTERM)


@app.get("/update/")
async def update_cypress():
    return {"updating": True}


if __name__ == "__main__":
    multiprocessing.freeze_support()  # for ms windows to work
    uvicorn.run(
        app,
        host=config.host,
        port=config.port,
        #log_level="info",
        #log_config=LOGGING_CONFIG,
        ssl_certfile="build/server.crt",
        ssl_keyfile="build/server.key",
    )
