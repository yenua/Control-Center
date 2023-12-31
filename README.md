Control Center Write-up
=============
Discription
-------------
XX 관제센터에서 XX을 관제하고 있다. 관제센터 직원들이 사고 발생을 알아차리지 못하도록 데이터를 변조하라!

[접속정보]  
http://{서버용 PC IP}:60080  
VNC : {서버용 PC IP}:5900  
VNC password : yamang

환경 구성
-------------
1. 도커 컴포즈 파일이 있는 곳에서 아래 명령어를 입력하여 도커를 띄운다.  
docker-compose up --build --force-recreate -d
2. rtsp 서버 도커에서 아래 명령어를 입력한다.  
./start.sh
3. 웹 사이트 및 vnc로 접속되는지 확인한다.

write-up
-------------

#### 0. 개요
해당 문제는 공격자가 내부망에 붙었다고 가정하고 문제 풀이가 시작된다.  
제공되는 웹 사이트에 접속 시 아래와 같은 화면이 출력된다.  
![Alt text](/img/관제센터%20평상시.png)
공격자가 관제 센터 화면을 볼 수 있는 상태에서, 관제센터 페이지에서 받아오는 영상(RTSP) 정보를 변조하여 잘못된 데이터를 받도록 하는 것이 문제의 목적이다.  
웹페이지에서 사고 발생 버튼을 눌렀을 때, 아래 사진과 같이 사고 발생이라는 아이콘이 영상에 뜨고 깜빡거리게 된다. (아직 미구현. RTSP서버에서 accident.sh를 실행시켜야 함.)  
![Alt text](/img/관제센터%20사고%20발생%20시.png)
공격자는 기존 화면을 녹화한 후, 본인 PC에서 해당 영상을 스트리밍 하고, ARP 스푸핑을 통해 웹서버에서 공격자의 PC에서 RTSP 영상을 가져가도록 하여 사고 발생 버튼을 눌렀음에도, 사고 발생 화면이 뜨지 않도록 해야 한다.  

#### 1. 포트스캔 진행  
![Alt text](/img/공격자%20ifconfig.png)
ifconfig를 했을 때, 172.36.0.0/24 대역에 붙었음을 확인할 수 있다.
이 때, 공격자 IP는 172.36.0.100으로 고정되어 있다.

![Alt text](/img/공격자%20서버에서%20호스트스캔한%20결과.png)
nmap -sn 172.36.0.0/24 를 이용하여 호스트 스캔을 해본 결과, 3개의 IP가 나온다.  
172.36.0.1 : Host PC  
172.36.0.2 : RTSP 서버
172.36.0.3 : 웹 서버

#### 2. ARP 스푸핑 진행
arpspoof -i eth0 -t 172.36.0.3 172.36.0.2 명령어를 이용해 웹서버가 공격자 서버로부터 영상을 받아가도록, RTSP 서버에 대한 정보를 변조시킨다.

![Alt text](/img/arp테이블%20원본.png)
[ARP 스푸핑 이전 웹서버 ARP 테이블]  
![Alt text](/img/arp스푸핑%20이후%20테이블.png)
[ARP 스푸핑 이후 웹서버 ARP 테이블]  

안된다..!!
  

![Alt text](/img/웹서버에서%20공격자%20서버로는%20통신이%20됨.png)
웹서버에서 공격자의 웹페이지로 접속은 되는 것을 보아, 웹서버에서 공격자 서버로는 양방향으로 통신이 되는 것을 확인하였는데, 테이블 변조까진 된거 같은데.. 실제 네트워크 패킷을 가로채진 못하는거 같다....ㅠㅠ



Issue
-------------
1. 칼리로 공격자용 PC를 제공하려고 하였으나.. 툴 하나 설치하는데 1시간 30분 걸림.. → 우분투로 변경.(잘되는 것을 보아하니 네트워크나, 리소스 이슈가 아니라 칼리 서버의 문제였던 것으로 보임)
2. 우분투에 뭐가 아무것도 없음; 편의를 위해 왠만한 툴들은 다 설치를 해줘야 할지.. 알아서 하게 해야할지..(공격툴/스트리밍 툴)
3. 우분투(172.36.0.100)에서 웹서버(172.36.0.3)가 나를 rtsp 서버(172.36.0.2)인 것으로 착각하게 만들게 arpspoofing 를 해도, 공격자가 보낸 것으로 보이지않음.(cmd: arpspoof -i eth0 -t 172.36.0.3 172.36.0.2) 게이트웨이를 만들어줘야 하나..?  브릿지가 문제인가!?
    1) arpspoof -i eth0 -t 172.36.0.3 172.36.0.2  
    arpspoof -i eth0 -t 172.36.0.2 172.36.0.3  
    혹시 몰라서 이렇게 둘다 동시에 해봐도 안됨.
    2) 도커에서 게이트웨이는 호스트인 172.36.0.1이라고 함.  
    arpspoof -i eth0 -t 172.36.0.3 172.36.0.1  
    arpspoof -i eth0 -t 172.36.0.1 172.36.0.2  
    이렇게 해봐도 안됨...ㅠ 함만 더해볼까!?
    3) ARP 캐시가 문제이고, 운영체제 마다 다를 수 있으나 3~5분 뒤에는 캐시가 만료되어 될 수 있다는 GPT의 의견이 있어서, 기다렸다가 확인해보았으나 RTSP 서버로 잘만 접속함..ㅋㅋ;
    4) arp -d 로 지우고 해봐도 안됨.. 스푸핑 자체가 안된건가? 근데 왜 테이블은 바뀜? ;;;
    5) cap_add도 안됨.
    6) 디폴트 네트워크에서도 안됨..

To do
-------------
0. 풀이 검증.......
1. 사고발생 눌렀을 때, 사고발생 영상이 뜨도록 구현하기(현재는 rtsp 도커에서 수동으로 ./accident.sh 실행시켜야 함.)
2. 웹 사이트 한눈에 보이게 세팅하기
3. (선택) 종민 멘토님이 알려주신 사이트(http://211.48.17.181:8084/)처럼 도로침수 통합관리 시스템이든, 실시간 도시방범 시스템이든, 그럴싸한 관제 시스템처럼 보이게 꾸며야 할까?
4. (선택) 공격자용 우분투 도커에, rtsp 스트리밍을 위한 세팅은 해줘야 할까?(해커는 원래 자기가 다 알아서 해야하는거긴한데.. 보안의 영역은 아닌거 같은데)
5. (선택) 공격자용 우분투 도커에, ftp든 scp든 파일전송 프로토콜용 포트를 오픈해두어야 할까?(현재는 인터넷 이용하여 필요한 파일 다운받고 있음)

VM 셋팅
-------------
1. Lubuntu  
    1) Install Lubuntu 22.04 LTS 실행
    2) 화면 보호기 해제 - 윈도우 - Preferences - Screensaver - Disable Screen Saver
    2) 업데이트(의존성 문제 예방)
        - sudo apt-get update
        - sudo apt-get upgrade


메모장
-------------
python3 -m http.server 9000

python3 -m http.server 9000 --bind 192.168.3.235

arpspoof -i ens33 -t 192.168.3.223 192.168.3.222

arpspoof -i ens33 -t 192.168.1.223 192.168.1.222

arpspoof -i ens33 -t 192.168.1.23 192.168.1.22

arpspoof -i ens33 -t 192.168.210.129 192.168.210.130

arpspoof -i ens33 -t 192.168.210.129 192.168.210.130

arpspoof -i ens33 -t 192.168.183.138 192.168.183.137

arpspoof -i eth0 -t 172.36.0.3 172.36.0.2 &

arpspoof -i eth0 -t 172.36.0.4 172.19.0.3
arpspoof -i eth0 -t 172.20.0.4 172.20.0.2

arpspoof -i eth0 -t 172.36.0.3 172.36.0.1 &
arpspoof -i eth0 -t 172.36.0.1 172.36.0.2 &

arpspoof -i eth0 -t 172.36.0.3 172.36.0.2  
arpspoof -i eth0 -t 172.36.0.2 172.36.0.3 
=> arpspoof -i eth0 -t 172.36.0.3 172.36.0.2 -r 

docker-compose up --build --force-recreate -d

docker exec -it id bash

kali ip: 192.168.183.128

Is YamangCity really safe?
HACKTIZEN{15_y4m@ngc1ty_re411_5@fe?}

IP alias
- cd /etc/sysconfig/network-scripts

나중에 하자...
apt-get dist-upgrade

네트워크 설정하는 방법 알려주기..
- vm 네트워크 관리자, 루분투 네트워크 설정

칼리 한국 서버로 변경
sudo nano /etc/apt/sources.list
http://mirror.amuksa.com/kali

cain & abel 
https://www.youtube.com/watch?v=Y2fhWtZedTQ

References
-------------
https://www.caverion.com/smart-city/  
https://github.com/aler9/rtsp-simple-server/releases  
https://tkdguq0110.tistory.com/entry/fragrouter

도커에서 된다는데..  
https://dockersec.blogspot.com/2017/01/arp-spoofing-docker-containers_26.html?source=post_page-----79f345ff721f--------------------------------