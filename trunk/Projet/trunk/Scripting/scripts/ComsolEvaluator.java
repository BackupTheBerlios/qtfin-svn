import com.femlab.script.api.*;
import java.io.*;
import java.lang.StringBuilder;
import java.util.regex.*;

public class ComsolEvaluator {

	private static void processException(Exception e) {
		System.out.println("@@@ Error:");
		System.out.println(e.getMessage());
		System.exit(1);
	}

	public static void main(String[] args) {
		// builder that will contain the one line script
		StringBuilder script = new StringBuilder();
		
		System.out.println("@@@ Reading input COMSOL script...");
		try {
			// retrieve a reader for System.in
			BufferedReader reader = new BufferedReader(new InputStreamReader(
					System.in));

			// regex to strap the multiline instructions
			Pattern miPattern = Pattern.compile("(.*)\\.\\.\\.\\s*$");
			
			// iteration throw System.in
			String line = "";
			while ((line = reader.readLine()) != null) {			
				Matcher miMatcher = miPattern.matcher(line);

				// multiline instruction ?
				if (miMatcher.matches())
					script.append(miMatcher.group(1));
				else {
					// classic line
					script.append(line);
					script.append(System.getProperty("line.separator"));
				}
				
			}

			reader.close();
		} catch (IOException e) {
			processException(e);
		}

		System.out.println();
		System.out.println("@@@");
		System.out.println(script.toString());
		System.out.println("@@@");
		System.out.println();

		System.out.println("@@@ Evaluating input COMSOL script...");
		try {
			// create the COMSOL workspace
			ApiWorkspace ws = new ApiWorkspace();

			// evaluation
			ws.eval(script.toString());
			
			// delete the COMSOL workspace
			ws.destroy();
		} catch (ApiException e) {
			processException(e);
		}
		
		System.out.println("@@@ Done!");
		System.exit(0);
	}

}
