FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies from install.sh and the packages needed for the GUI
RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc \
    python3 \
    python3-pip \
    perl \
    make \
    gdb \
    valgrind \
    imagemagick \
    rustc \
    cargo \
    pkg-config \
    libgtk-4-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy the entire project into the container
COPY . /app
WORKDIR /app

# Build the main CLI and GUI applications using the Makefile
RUN make

# Set the default command to run the GUI application when the container starts
CMD ["./bin/image-gui"]

