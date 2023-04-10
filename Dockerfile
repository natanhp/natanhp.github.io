# get and configure an image for building our app
FROM debian:latest as builder
RUN apt-get update && apt-get upgrade -yy
RUN apt-get install -y --no-install-recommends \
    cmake \
    build-essential \
    ca-certificates \
    libboost-all-dev \
    libssl-dev \
    wget \
    zlib1g-dev

# get and build wt4
WORKDIR /root
RUN wget https://github.com/emweb/wt/archive/4.7.1.tar.gz
RUN tar zxf 4.7.1.tar.gz
WORKDIR /root/wt-4.7.1/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF -DSHARED_LIBS=OFF ..
RUN make -j2 install

# build our app (static linking)
WORKDIR /root/myrepeat
ADD . /root/myrepeat/
RUN cmake -DCMAKE_BUILD_TYPE=Release.
RUN cmake --build .

# create the final image
FROM debian:latest
RUN apt-get update && apt-get upgrade -yy
WORKDIR /root
COPY --from=builder /root/myrepeat/natanhpid.wt /root/
COPY --from=builder /root/myrepeat/resources/ /root/
CMD /root/myrepeat --docroot . --http-address 0.0.0.0 --http-port 8080