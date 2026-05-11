import psutil
import uvicorn
import multiprocessing
import asyncio

from fastapi import FastAPI, Request, status
from fastapi import BackgroundTasks
from fastapi.responses import FileResponse, JSONResponse

from settings import LOGGING_CONFIG, ALLOWED_IPS

from config import config
from session import Session

from devices.device import Device
from devices.crt import ChoiceReactionTest, CRTSession
from devices.cdtt import CDTT, CDTTSession
from devices.frax import FRAX, FRAXSession

# The web server
app = FastAPI()

# The device currently opened
current_session: dict[str, multiprocessing.Process] | None = None

devices: dict[str, Device] = {
    #"hearcon": "",
    #"watch_bp": "",
    "cdtt": CDTT,
    "choice_reaction_test": ChoiceReactionTest,
    #"dxa1": "",
    #"dxa2": "",
    #"mac5": "",
    #"vivid_iq": "",
    "frax": FRAX,
    #"general_proxy_consent": "",
    #"hand_grip": "",
    #"oct_left": "",
    #"oct_right": "",
    #"easyone_connect": "",
    #"ora": "",
    #"weight_scale": "",
}


@app.middleware("http")
async def validate_ip(request: Request, call_next):
    ip = str(request.client.host)

    if ip not in ALLOWED_IPS:
        data = {"message": f"{ip} is not allowed to access this resource."}
        return JSONResponse(status_code=status.HTTP_400_BAD_REQUEST, content=data)

    return await call_next(request)


async def monitor_session():
    global current_session
    while True:
        if not psutil.pid_exists(current_session["process"].pid):
            current_session = None
            break
        await asyncio.sleep(1)


def set_session(device: str, session: Session):
    global current_session

    device_process = multiprocessing.Process(target=devices[device].run, args=(session,))
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

def launch_device(device_name: str, background_tasks: BackgroundTasks, session: Session):
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
    return {"device": device_name, "pid": current_session["process"].pid}


@app.get("/", response_class=FileResponse)
async def index():
    return FileResponse(path="./index.html", media_type="text/html")

@app.post("/device/cdtt")
async def cdtt(background_tasks: BackgroundTasks, session: CDTTSession):
    return launch_device("cdtt", background_tasks, session)

@app.post("/device/choice_reaction_test")
async def cdtt(background_tasks: BackgroundTasks, session: CRTSession):
    return launch_device("choice_reaction_test", background_tasks, session)

@app.post("/device/frax")
async def cdtt(background_tasks: BackgroundTasks, session: CRTSession):
    return launch_device("frax", background_tasks, session)

@app.post("/update/")
async def update():
    # if current_session:
    return {"error": "session in progress"}


@app.get("/{device}/status")
async def get_status(device: str):
    if device not in devices:
        return {"error": "unsupported device"}

    global current_session
    if not current_session:
        return {"status": "available"}

    return {"device": current_session["device"], "pid": current_session["process"].pid}


@app.get("/update/")
async def update_cypress():
    return {"updating": True}


if __name__ == "__main__":
    multiprocessing.freeze_support()  # for ms windows to work
    uvicorn.run(app, host=str(config.host), port=config.port, log_config=LOGGING_CONFIG)
