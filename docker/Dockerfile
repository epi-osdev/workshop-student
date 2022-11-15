FROM i686/ubuntu:latest

# Update from apt sources files and upgrade packages
# + download needed dependencies
RUN apt-get update
RUN apt-get install -y make \
    wget \
    unzip \
    nasm \
    lbzip2 \
    grub-common

# Re-used environment variables
ENV DIR="/osdev"
ENV I686="i686-elf-tools-linux"
ENV I686OUT="/opt/${I686}"

# Working directory of the container
WORKDIR ${DIR}

# Install the i686 toolchain

# Download pre-built i686 binaries (GCC + linker)
RUN wget -P /tmp "https://github.com/lordmilko/i686-elf-tools/releases/download/7.1.0/${I686}.zip"
# Extract the files to the i686 tool directory
RUN unzip -d "${I686OUT}" "/tmp/${I686}"

# Create an user (avoid root)
RUN useradd -ms /bin/bash theo
USER theo

# Update PATH
ENV PATH="${PATH}:${I686OUT}/bin"

ENTRYPOINT ["/bin/bash"]
