FROM alpine:latest
RUN apk update && apk upgrade
WORKDIR /root
RUN mkdir bin
COPY ./bin /root/bin
CMD /root/bin/natanhpid.wt -c /root/bin/resources/wt_config.xml --docroot /root/bin/. --http-address 0.0.0.0 --http-port 8080
