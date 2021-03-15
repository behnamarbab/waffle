FROM ubuntu:xenial

# RUN cp /etc/apt/sources.list /etc/apt/sources.list.bak
# RUN sed -i s:/archive.ubuntu.com:/mirrors.tuna.tsinghua.edu.cn/ubuntu:g /etc/apt/sources.list
# RUN apt-get clean

RUN apt-get update 
RUN apt-get install -y wget apt-utils git build-essential tmux cmake libtool automake autoconf autotools-dev m4 autopoint help2man bison flex texinfo zlib1g-dev libexpat1-dev libfreetype6 libfreetype6-dev --fix-missing


# RUN apt-get update
RUN apt-get install software-properties-common -y
RUN apt-get install apt-transport-https -y
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN apt-add-repository "deb http://apt.llvm.org/xenial/ llvm-toolchain-xenial-6.0 main" -y
RUN apt-get update
RUN apt-get install -y clang llvm

COPY . /source_file
WORKDIR /source_file

# ENV PATH "/workdir/MemLock/clang+llvm/bin:$PATH"
# ENV LD_LIBRARY_PATH "/workdir/MemLock/clang+llvm/lib:$LD_LIBRARY_PATH"

# RUN tool/install_llvm.sh
RUN cd waffle && CC=clang make && cd llvm_mode && make && echo $(clang -v)