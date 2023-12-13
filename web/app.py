from flask import Flask, render_template, Response
import cv2
#Initialize the Flask app
app = Flask(__name__)

#camera = cv2.VideoCapture('rtsp://localhost:8554/live')

def gen_frames():
    print('help')
    #camera = cv2.VideoCapture('rtsp://172.36.0.2:60554/live')
    #camera = cv2.VideoCapture('rtsp://172.19.0.2:60554/live')
    #camera = cv2.VideoCapture('rtsp://127.0.0.1:60554/live')
    camera = cv2.VideoCapture('rtsp://192.168.3.222:60554/live')
    while True:
        success, frame = camera.read()  # read the camera frame
        if not success:
            break
        else:
            ret, buffer = cv2.imencode('.jpg', frame)
            frame = buffer.tobytes()
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n') 
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/video_feed')
def video_feed():
    return Response(gen_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == "__main__":
    # app.run(debug=True, host='0.0.0.0')
    app.run(host='0.0.0.0', port=5000)