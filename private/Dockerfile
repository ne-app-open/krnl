FROM debian

# Update & upgrade APT.

RUN apt-get update && apt-get upgrade -y

# Blobs for private NeKernel parts.
RUN mkdir /blobs

COPY ./ ./src

RUN echo "Ne.app Kernel: Contents have been copied to Docker."

# Reserved for Ne.app Kernel Debugger Protocol
EXPOSE 8081/tcp

# For the Ne.app Kernel Debugger Protocol as well (UDP)
EXPOSE 8081/udp
