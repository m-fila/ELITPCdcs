FROM mfila/open62541:1.1.1
ADD . /opt/dcs
WORKDIR /opt/dcs
RUN rm -rf build
RUN cmake -H. -Bbuild -DBUILD_GUI=OFF
RUN cmake --build build --target install -j
