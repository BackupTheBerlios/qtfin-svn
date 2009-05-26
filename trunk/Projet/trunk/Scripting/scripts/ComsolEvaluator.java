import com.femlab.script.api.*;
import java.io.*;

public class ComsolEvaluator {

	/**
	 * End Of Script.
	 */
	private static final String EOS = "@@@";

	/**
	 * New line.
	 */
	private static final String NEWLINE = System.getProperty("line.separator");

	/**
	 * Try to create a log file.
	 * 
	 * @return Instance of FileWriter or null.
	 */
	private static FileWriter createLogWriter() {
		FileWriter writer = null;
		try {
			writer = new FileWriter("ComsolEvaluator.log");
		} catch (IOException e) {
			e.printStackTrace();
		}
		return writer;
	}

	/**
	 * Try to write a string ending with a new line in a file.
	 * 
	 * @param log
	 * @param message
	 */
	private static void writeLog(FileWriter log, String message) {
		if (log == null)
			return;

		try {
			log.write(message);
			log.write(NEWLINE);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void main(String[] args) {
		// error status
		int status = 0;
		
		// log
		FileWriter log = createLogWriter();

		writeLog(log, "Reading input COMSOL script...");

		// create the COMSOL workspace
		ApiWorkspace ws = new ApiWorkspace();
		BufferedReader reader = null;

		try {
			// reading System.in
			reader = new BufferedReader(new InputStreamReader(System.in));

			// a line from System.in
			String line = "";

			// iteration throw System.in
			while ((line = reader.readLine()) != null) {

				if (line.equals(EOS)) {
					writeLog(log, "EOS");
					break;
				}

				line = line.trim().replaceAll(NEWLINE, "");
				writeLog(log, "eval: " + line);
				ws.eval(line);
			}

		} catch (IOException e) {
			e.printStackTrace();
			writeLog(log, e.getMessage());
			status = 1;
		} catch (ApiException e) {
			e.printStackTrace();
			writeLog(log, e.getMessage());
			status = 1;
		} finally {
			// disposing ws
			if (ws != null)
				try {
					ws.destroy();
				} catch (ApiException e) {
					e.printStackTrace();
				}
			// disposing reader
			if (reader != null)
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}

		writeLog(log, "Done!");
		if (log != null)
			try {
				log.close();
			} catch (IOException e) {
				e.printStackTrace();
			}

		System.exit(status);
	}
}
