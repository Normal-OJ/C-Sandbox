# for build

FROM ubuntu:18.04

MAINTAINER Normal-OJ

# update
RUN apt-get update -y

# install seccomp
RUN apt-get install libseccomp-dev libseccomp2 seccomp -y

# install g++
RUN apt-get install build-essential -y