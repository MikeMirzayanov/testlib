#!/bin/bash

uname_output="$(uname -s)"
case "${uname_output}" in
Linux*) machine=Linux ;;
Darwin*) machine=Mac ;;
CYGWIN*) machine=Windows ;;
MINGW*) machine=Windows ;;
MSYS*) machine=Windows ;;
*) echo "Unknown system '${uname_output}'" && exit 1 ;;
esac

# If JAVA8_32_HOME is set, use its javac
if [[ -n "$JAVA8_32_HOME" ]]; then
    if [[ "$machine" == "Windows" ]]; then
        JAVA8_32_HOME=$(cygpath "$JAVA8_32_HOME")
    fi
    export PATH="$JAVA8_32_HOME/bin:$PATH"
fi

# If JAVA7_32_HOME is set, use its javac
if [[ -n "$JAVA7_32_HOME" ]]; then
    if [[ "$machine" == "Windows" ]]; then
        JAVA7_32_HOME=$(cygpath "$JAVA7_32_HOME")
    fi
    export PATH="$JAVA7_32_HOME/bin:$PATH"
fi

echo $PATH

# Show javac version
javac -version

# Compile all .java files
javac *.java

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Error during compilation. Exiting."
    exit 1
fi

# Create a manifest file for the jar
echo "Main-Class: CrossRun" > manifest.mf

# Package all .class files into a jar
jar cvfm CrossRun.jar manifest.mf *.class

# Cleanup .class files and manifest file (optional)
rm *.class
rm manifest.mf

echo "Done. CrossRun.jar created."
