FROM silkeh/clang:11
RUN apt-get update
RUN apt-get install -y git default-jre
COPY startup.sh /
WORKDIR /
RUN chmod +x /startup.sh
CMD ["/bin/bash", "/startup.sh"]
