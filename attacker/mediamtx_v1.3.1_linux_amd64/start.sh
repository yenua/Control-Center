./mediamtx &

sleep 5

nohup ffmpeg -re -stream_loop -1 -i caverion_smartcity_cityview_v2_2.mp4 -c copy -f rtsp rtsp://localhost:60554/live &