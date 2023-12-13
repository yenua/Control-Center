./mediamtx &

sleep 5

nohup ffmpeg -re -stream_loop -1 -i target_cctv.mp4 -c copy -f rtsp rtsp://localhost:60554/live &