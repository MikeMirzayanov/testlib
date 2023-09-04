import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class CrossRun {
    private static final List<String> messages = Collections.synchronizedList(new ArrayList<String>());
    
    private static volatile boolean failed = false;

    private static void error(String message) {
        System.out.println("ERROR: " + message);
        System.exit(1);
    }

    public static void main(String[] args) {
        int sep = -1;
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("--")) {
                sep = i;
                break;
            }
        }
        
        if (sep == -1) {
            error("Expected exactly one '--' as separator for the two process command lines.");
        }
        
        String[] params1 = new String[sep];
        System.arraycopy(args, 0, params1, 0, params1.length);
        
        String[] params2 = new String[args.length - sep - 1];
        System.arraycopy(args, sep + 1, params2, 0, params2.length);
        
        long startTime = System.currentTimeMillis();

        try {
            runProcesses(params1, params2);
        } catch (IOException e) {
            error(e.getMessage());
        }

        System.out.println("Completed in " + (System.currentTimeMillis() - startTime) + " ms.");
    }

    private static void runProcesses(String[] params1, String[] params2) throws IOException {
        Process process1 = new ProcessBuilder(params1).start();
        Process process2 = new ProcessBuilder(params2).start();

        Thread readProcess1WriteProcess2Thread =
                new Thread(new StreamProxyRunner("process1", "process2", process1.getInputStream(), process2.getOutputStream()));
        Thread readProcess2WriteProcess1Thread =
                new Thread(new StreamProxyRunner("process2", "process1", process2.getInputStream(), process1.getOutputStream()));

        readProcess1WriteProcess2Thread.start();
        readProcess2WriteProcess1Thread.start();

        int processExitCode1 = -1;
        try {
            processExitCode1 = process1.waitFor();
        } catch (InterruptedException e) {
            error(e.getMessage());
        }

        int processExitCode2 = -1;
        try {
            processExitCode2 = process2.waitFor();
        } catch (InterruptedException e) {
            error(e.getMessage());
        }

        try {
            readProcess1WriteProcess2Thread.join();
        } catch (InterruptedException e) {
            error(e.getMessage());
        }

        try {
            readProcess2WriteProcess1Thread.join();
        } catch (InterruptedException e) {
            error(e.getMessage());
        }

        if (processExitCode1 != 0) {
            messages.add("The process 1 returned with exit code " + processExitCode1 + ".");
        }

        if (processExitCode2 != 0) {
            messages.add("The process 2 returned with exit code " + processExitCode2 + ".");
        }

        for (String message : messages) {
            System.out.println("* " + message);
        }

        if (failed) {
            System.exit(1);
        }
    }

    @SuppressWarnings("ClassCanBeRecord")
    private static final class StreamProxyRunner implements Runnable {
        private final String processName1;
        private final String processName2;
        private final InputStream inputStream;
        private final OutputStream outputStream;

        private StreamProxyRunner(final String processName1,
                                  final String processName2,
                                  final InputStream inputStream,
                                  final OutputStream outputStream) {
            this.processName1 = processName1;
            this.processName2 = processName2;
            this.inputStream = inputStream;
            this.outputStream = outputStream;
        }

        @Override
        public void run() {
            byte[] buffer = new byte[65536];

            while (true) {
                int size;

                try {
                    size = inputStream.read(buffer);
                } catch (IOException e) {
                    messages.add("Unexpected exception " + e.getClass().getSimpleName() + " while reading from the output of the " + processName1 + " process: " + e.getMessage());
                    failed = true;
                    break;
                }

                if (size < 0) {
                    break;
                }

                try {
                    outputStream.write(buffer, 0, size);
                    outputStream.flush();
                } catch (IOException e) {
                    messages.add("Unexpected exception " + e.getClass().getSimpleName() + " while writing to the input of the " + processName2 + " process: " + e.getMessage());
                    failed = true;
                    break;
                }
            }

            try {
                inputStream.close();
            } catch (IOException e) {
                // No operations.
            }

            try {
                outputStream.close();
            } catch (IOException e) {
                // No operations.
            }
        }
    }
}
