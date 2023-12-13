FROM dorowu/ubuntu-desktop-lxde-vnc

ENV TZ=Asia/Seoul

# InRelease: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 4EB27DB2A3B88B8B 오류 해결용
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 4EB27DB2A3B88B8B

RUN apt-get update -y

RUN apt-get install -y net-tools
RUN apt-get install -y iputils-ping

# 지울까 말까
RUN apt-get install -y nmap
RUN apt-get install -y dsniff
#fragrouter가 없대..
#RUN apt-get install -y fragrouter
#RUN apt-get install -y python3-pip
RUN apt-get install -y netcat
RUN apt-get install -y wireshark
COPY ./mediamtx_v1.3.1_linux_amd64 /root/Desktop/rtsp

COPY ./fragrouter-1.6 /root/Desktop/arp
RUN apt-get install bison -y
# RUN apt-get install yacc -y
# RUN apt-get install flex -y