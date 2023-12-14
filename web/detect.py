from flask import Flask, render_template, Response
import cv2
import numpy as np
import time

#Initialize the Flask app
app = Flask(__name__)

# Load the template image for matching
template = cv2.imread('./static/img/accident.png', cv2.IMREAD_COLOR)
template_gray = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)

start_time = time.time()
current_time = time.time()
delay_start_time = None
status = False

def gen_frames():
    global start_time
    global current_time
    global delay_start_time 
    print(start_time, current_time)
    #camera = cv2.VideoCapture('rtsp://172.36.0.2:60554/live')
    #camera = cv2.VideoCapture('rtsp://172.19.0.2:60554/live')
    #camera = cv2.VideoCapture('rtsp://127.0.0.1:60554/live')
    camera = cv2.VideoCapture('rtsp://192.168.3.222:60554/live')
    print('help1')
    
    while True:
        success, frame = camera.read()  # read the camera frame
        if not success:
            print('not success!')
            start_time = time.time()
            return
        else:
            ret, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()

            if delay_start_time == None:
                delay_start_time = time.time()
            delay_current_time = time.time()
            if delay_current_time - delay_start_time >= 3:
                # Match template and find locations of matches
                buffer = cv2.imdecode(buffer, cv2.IMREAD_COLOR)
                gray = cv2.cvtColor(buffer, cv2.COLOR_BGR2GRAY)
                res = cv2.matchTemplate(gray, template_gray, cv2.TM_CCOEFF_NORMED)
                threshold = 0.25
                loc = np.where(res >= threshold)
                
                # 이미지 검출 임계값 확인용
                # minVal, maxVal, minLoc, maxLoc = cv2.minMaxLoc(res)
                # with open("log.txt", "a") as log_file:
                #     print(minVal, maxVal, minLoc, maxLoc, file=log_file)

                # Check if the template is found
                if len(loc[0]) > 0:
                    print('accident!')
                    # Set a flag to indicate image detection
                    start_time = time.time()
                else:
                    print('not accident!')
                    current_time = time.time()
                
                delay_start_time = time.time()

            yield (b'--frame\r\n'
                b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
            
@app.route('/')
def index():
    global start_time
    global current_time
    global status
    if current_time - start_time >= 60:
        status = True
    if status == True:
        print('flag!')
        return render_template('index2.html', flag='HACKTIZEN{15_y4m@ngc1ty_re411_5@fe?}')
    else:
        print('not flag!')
        return render_template('index2.html', flag='HACKTIZEN{fake_flag}')

@app.route('/video_feed')
def video_feed():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == "__main__":
    # app.run(debug=True, host='0.0.0.0')
    app.run(host='0.0.0.0', port=5000)