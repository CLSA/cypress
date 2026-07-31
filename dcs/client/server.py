def run_server():
    import uvicorn

    from fastapi import FastAPI

    from fastapi.middleware.httpsredirect import HTTPSRedirectMiddleware
    from fastapi.middleware.trustedhost import TrustedHostMiddleware

    from settings import ALLOWED_HOSTS, LOGGING_CONFIG

    from routes import router
    from config import config

    app = FastAPI()
    #app.add_middleware(HTTPSRedirectMiddleware)
    app.add_middleware(TrustedHostMiddleware, allowed_hosts=ALLOWED_HOSTS)

    app.include_router(router=router)

    uvicorn.run(
        app=app,
        host=config.host,
        port=config.port,
        log_config=LOGGING_CONFIG,
        ssl_certfile=config.ssl_certfile,
        ssl_keyfile=config.ssl_keyfile,
    )
