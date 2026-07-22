from fastapi import FastAPI
from fastapi.responses import FileResponse

app = FastAPI()


@app.get("/cypress")
async def root():
    pass