FROM dorowu/ubuntu-desktop-lxde-vnc

ENV TZ=Asia/Seoul

# InRelease: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 4EB27DB2A3B88B8B 오류 해결용
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4EB27DB2A3B88B8B

RUN apt-get update -y

RUN apt-get install -y net-tools
RUN apt-get install iputils-ping

RUN apt-get install -y nmap
RUN apt-get install -y dsniff