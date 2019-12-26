# for build

FROM alpine

MAINTAINER Normal-OJ

# install essential
RUN apk add --update alpine-sdk

#install seccomp
RUN apk add --update libseccomp-dev

