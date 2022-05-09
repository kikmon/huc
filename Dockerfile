#FROM alpine:3.15.4 AS Build
#RUN apk update && apk add --virtual build-dependencies build-base gcc wget git

FROM ubuntu:latest AS Build
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

ADD . /app
WORKDIR /app
RUN make
#have a look at snyk

#FROM alpine:3.15.4
FROM ubuntu:latest

#RUN useradd -ms /bin/bash docker
#USER newuser
#WORKDIR /home/newuser

COPY --from=Build /app/bin/ /huc/bin/
COPY --from=Build /app/include/ /huc/include/
ENV PATH="/huc/bin:${PATH}"
ENV PCE_INCLUDE="/huc/include/pce"

# subtle differences between shell and executable forms concerning arguments and * expansions
WORKDIR /src
ENTRYPOINT ["huc"]





