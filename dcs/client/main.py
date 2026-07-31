def main():
    import multiprocessing

    multiprocessing.freeze_support()  # for ms windows to work

    from server import run_server

    run_server()


if __name__ == "__main__":
    main()
