FROM python:3.11

COPY . /app

RUN pip install --upgrade pip
RUN pip install flask
RUN pip install opencv-python

RUN apt-get update
RUN apt-get install libgl1-mesa-glx -y
RUN apt-get install net-tools -y

WORKDIR /app

CMD ["python3", "app.py"]