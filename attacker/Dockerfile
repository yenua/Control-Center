FROM kalilinux/kali-rolling

ENV TZ=Asia/Seoul

RUN apt-get update

# nmap 포함 칼리 추천 상위 10개 도구 설치
RUN apt-get install -y kali-tools-top10

# 데스크탑 환경 설치
RUN apt-get install -y  kali-desktop-xfce

# VNC 서버 설치
RUN apt-get install -y tightvncserver
