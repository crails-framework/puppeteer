FROM ubuntu:19.10

ENV LD_LIBRARY_PATH /usr/local/lib

RUN apt-get -y --allow-unauthenticated update && \
    apt-get -y --allow-unauthenticated upgrade && \
    apt-get -y install wget \
  cmake \
  build-essential \
  libbz2-dev \
  libssl-dev \
  libssh-dev \
  git \
  ruby \
  ruby-bundler \
  ruby-dev \
  python \
  nodejs

# Downgrade to gcc-7 to solve compatibility issues with odb 2.5
RUN apt-get -y install gcc-7 g++-7 gcc-7-plugin-dev
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 20
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 20

WORKDIR /tmp/compile-dir

COPY libgit.sh libgit.sh
RUN bash /tmp/compile-dir/libgit.sh

# BEGIN Crails dependencies
COPY boost.sh boost.sh
RUN bash /tmp/compile-dir/boost.sh

COPY cppnetlib.sh cppnetlib.sh
RUN bash ./cppnetlib.sh

COPY fetch-cheerp.sh fetch-cheerp.sh
RUN bash ./fetch-cheerp.sh

COPY compile-cheerp.sh compile-cheerp.sh
RUN bash ./compile-cheerp.sh

COPY compile-odb.sh compile-odb.sh
RUN bash ./compile-odb.sh
# END Crails dependencies

COPY crails.sh crails.sh
RUN bash ./crails.sh
ENV CRAILS_SHARED_DIR /usr/local/share/crails

# BEGIN Dependencies
RUN apt-get install -y npm
# END Dependencies

RUN mkdir -p /opt/webapp
WORKDIR /opt/webapp
ENV CRAILS_RUBY_BUNDLE_PATH /opt/webapp/docker
