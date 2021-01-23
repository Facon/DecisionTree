FROM debian:latest

RUN apt update && apt -y dist-upgrade

RUN apt install -y clang python3-pip cmake
RUN update-alternatives --set cc /usr/bin/clang && \
update-alternatives --set c++ /usr/bin/clang++

RUN useradd -u 10000 -U -s /bin/bash -m nonroot

USER nonroot
WORKDIR /home/nonroot

RUN pip3 install conan
ENV PATH="/home/nonroot/.local/bin:${PATH}"
RUN conan profile new --detect clang && \
conan profile update settings.compiler.libcxx=libstdc++11 clang && \
conan profile update settings.compiler=clang clang && \
conan profile update settings.compiler.version=7.0 clang && \
conan remote list

COPY . ./DecisionTree
RUN mkdir build-DecisionTree-Release
RUN cat .conan/profiles/clang
RUN cd build-DecisionTree-Release && \
conan install ../DecisionTree -pr=clang -b=missing && \
cmake ../DecisionTree -DCMAKE_BUILD_TYPE=Release && \
cmake --build . --target all

CMD ["/bin/bash"]
