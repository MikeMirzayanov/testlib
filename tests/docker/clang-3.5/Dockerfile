FROM silkeh/clang:3.5
RUN sed -i -e 's|deb.debian.org|archive.debian.org|g' -e 's|security.debian.org|archive.debian.org|g' /etc/apt/sources.list && apt-get update
RUN apt-get install -y git default-jre
COPY startup.sh /
WORKDIR /
RUN chmod +x /startup.sh
CMD ["/bin/bash", "/startup.sh"]
