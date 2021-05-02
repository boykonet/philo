FROM ubuntu:latest
RUN apt update
RUN apt install -y valgrind gcc make

CMD tail -f /dev/null

