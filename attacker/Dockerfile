# From the official Kali Linux Docker image rolling release
FROM kalilinux/kali-rolling
# Leaving this here for now, but it's not needed
ARG DISPLAY_NUMBER=1
ENV DISPLAY=:$DISPLAY_NUMBER

# Install the packages we need
# Got lucky with this one, there was a ticket about this:
# https://github.com/dnschneid/crouton/issues/4461 `xfce4 dbus-launch not found` in this version of the image so installing dbus-x11
RUN apt-get update -y -q \ && apt-get install -y -q xvfb xfce4 xfce4-goodies tightvncserver net-tools curl openssh-server dbus-x11
# This line copies the shell scripts from the local directory to the container, and makes them executable.
COPY *.sh /opt/ RUN chmod 755 /opt/*.sh
# This line runs the shell scripts that start the X server, and generate the SSH keys.
RUN /opt/startx-once.sh RUN /opt/ssh-keys.sh
# This line sets the entrypoint to bash, so that we can run commands in the container.
ENTRYPOINT /bin/bash