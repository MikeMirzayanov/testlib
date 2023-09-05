FROM silkeh/clang:latest
RUN apt-get update
RUN apt-get install -y git default-jre valgrind
COPY startup.sh /
WORKDIR /
RUN chmod +x /startup.sh
CMD ["/bin/bash", "/startup.sh"]
