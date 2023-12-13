./mediamtx &

sleep 5

nohup ffmpeg -re -stream_loop -1 -i picture2-1.jpg -vf "fps=30" -f rtsp rtsp://localhost:60554/live &