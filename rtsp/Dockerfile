# 운영체제 ubuntu
FROM ubuntu:22.04
# FROM python:3.11

# 타임존 
ENV TZ=Asia/Seoul

# 설치시 사용자 입력을 요구하는 것을 방지하는 설정
# ENV DEBIAN_FRONTEND=noninteractive

# 필요 라이브러리 설치
# -y 옵션 : 설치 확인 여부를 yes로 자동 설정
RUN apt-get update
RUN apt-get install ffmpeg -y
RUN apt-get install net-tools -y

COPY ./mediamtx_v1.3.1_linux_amd64 /rtsp
COPY start.sh /etc/init.d

WORKDIR /rtsp

#CMD ["/bin/sh"]
# RUN ./mediamtx &
# RUN ffmpeg -re -stream_loop -1 -i caverion_smartcity_cityview_v2_2.mp4 -c copy -f rtsp rtsp://localhost:8554/live
# RUN nohup ffmpeg -re -stream_loop -1 -i caverion_smartcity_cityview_v2_2.mp4 -c copy -f rtsp rtsp://localhost:8554/live &

